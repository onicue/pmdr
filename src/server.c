#include "server.h"
#include "utils.h"
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

atomic_uint seconds = 10;
pthread_t* thread_id;

sockInfo server;

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
}

void close_server(){
  free(thread_id);
  close(server.sockfd);
  unlink(SOCKET_PATH);
  pthread_exit(NULL);
}

void *reverse_timer(void* arg){
  while(seconds){
    seconds--;
    sleep(1);
  }
}

void start_server(){
  int new_socket;
  char buffer[256];
  size_t valread;
  thread_id =(pthread_t*) malloc(sizeof(pthread_t));
  pthread_create(thread_id, NULL, reverse_timer, NULL);

  while (seconds) {
    if((new_socket = accept(server.sockfd, (struct sockaddr*)&server.address, &server.addrlen)) < 0){
      error_handling("Erro while accepting");
    }
    valread = read(new_socket, buffer, 256-1);
    buffer[valread] = '\0';
    printf("%s\n", buffer);
    itoa(seconds, buffer, 256);
    send(new_socket, buffer, strlen(buffer), 0);
    printf("sended message");
    close(new_socket);
  }
}


