#include "server.h"
#include "utils.h"
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool unsigned char
#define true 1
#define false 0

int default_work_time = DEFAULT_WORK_TIME; //sec
int default_rest_time = DEFAULT_REST_TIME; //sec
atomic_uint seconds;

static pthread_mutex_t mutex;
bool lock_flag = false;
bool close_flag = false;
pthread_t thread_id;

sockInfo server;
int client_socket;
char status[10] = "work";

void init_mutex(){
  if(pthread_mutex_init(&mutex, NULL) != 0){
    error_handling("Error while init mutex");
  }
}

void init_server(){
  server.sockfd = create_socket();
  server.address = create_sockaddr();
  server.addrlen = sizeof(server.address);
  if(bind(server.sockfd, (struct sockaddr*)&server.address, sizeof(server.address)) < 0){
    error_handling("Error while binding socket");
  }

  if(listen(server.sockfd, 10) < 0){
    error_handling("Error while listening on socket");
  }
  init_mutex();
}

void close_server(){
  close(server.sockfd);
  unlink(SOCKET_PATH);
  pthread_mutex_destroy(&mutex);
  pthread_cancel(thread_id);
}

void *reverse_timer(void* arg){
  seconds = default_work_time;
  while(true){
    while(seconds){
      if(lock_flag){
        pthread_mutex_lock(&mutex);
      }
      sleep(1);
      seconds--;
    }
    pthread_mutex_lock(&mutex);
    if(!strcmp(status, "work")){
      strcpy(status, "rest");
      seconds = default_rest_time;
    } else {
      strcpy(status, "work");
      seconds = default_work_time;
    }
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

void set_default_rest_time(int time){
  if(time > 0){
    default_rest_time = time;
  }
}


void set_default_work_time(int time){
  if(time > 0){
    default_work_time = time;
  }
}

void start_thread_loop(int work_time, int rest_time){
  set_default_work_time(work_time);
  set_default_rest_time(rest_time);
  pthread_create(&thread_id, NULL, reverse_timer, NULL);
}

void start_reverse_timer(int argc, char** argv){
  if(argc == 1) {
    start_thread_loop(0, 0);
  } else if ( argc == 2){
    start_thread_loop(atoi(argv[1]), 0);
  } else if (argc == 3){
    start_thread_loop(atoi(argv[1]), atoi(argv[2]));
  } else {
    perror("too many arguments in start");
  }
}

void send_time(){
  char nums[20];
  sprintf(nums, "%u", seconds);
  send_message(client_socket, nums);
}

void method_handling(td_array* array){
  int argc = array->len;
  char* method = array->lines[0];

  if(!strcmp(method, "start")){
    start_reverse_timer(argc, array->lines);
  } else if (!strcmp(method, "get") && argc == 1){
    send_time();
  } else if (!strcmp(method, "status") && argc == 1) {
    send_message(client_socket, status);
  } else if (!strcmp(method, "close") && argc == 1) {
    close_flag = true;
  } else if (!strcmp(method, "continue") && argc == 1) {
    lock_flag = false;
    pthread_mutex_unlock(&mutex);
  } else if (!strcmp(method, "stop") && argc == 1) {
    lock_flag = true;
  } else {
    perror("Error incorrect arguments");
  }
}

void message_handling(char* message){
  td_array array = separate_str(message);
  method_handling(&array);
  free_td_array(&array);
}

void start_server(){
  char buffer[BUFFER_SIZE];
  while (true) {
    if((client_socket = accept(server.sockfd, (struct sockaddr*)&server.address, &server.addrlen)) < 0){
      error_handling("Error while accepting");
    }
    recv_message(client_socket, buffer);
    message_handling(buffer);
    close(client_socket);
    if(close_flag){
      break;
    }
  }
}
