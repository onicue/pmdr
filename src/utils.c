#include "utils.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <string.h>

int create_socket(){
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(sockfd < 0){
    perror("Error while creating socket");
    exit(EXIT_FAILURE);
  }
  return sockfd;
}

struct sockaddr_un create_sockaddr(){
  struct sockaddr_un sockaddr;
  sockaddr.sun_family = AF_UNIX;
  strcpy(sockaddr.sun_path, SOCKET_PATH);
  return sockaddr;
}

void error_handling(const char* error_msg){
  perror(error_msg);
  exit(EXIT_FAILURE);
}

void send_message(int sockfd, char* message){
  if(send(sockfd, message, strlen(message) , 0) < 0){
    error_handling("Error while sending message");
  }
}

void recv_message(int sockfd, char *buffer){
  size_t bytes_recived = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
  if(bytes_recived < 0){
    error_handling("Error while receiving message");
  }
  buffer[bytes_recived] = '\0';
}

void add_token(td_array *array, int index, const char *start, int length) {
  array->lines[index] = malloc((length + 1) * sizeof(char));
  if (!array->lines[index]) {
    error_handling("malloc in function add_token");
  }
  strncpy(array->lines[index], start, length);
  array->lines[index][length] = '\0';
}

td_array separate_str(char *str) {
  td_array array = {0, NULL};
  int count = 1;
  for (char *p = str; *p != '\0'; p++) {
    if (*p == ' ') {
      count++;
    }
  }

  array.lines = malloc(count * sizeof(char *));
  if (!array.lines){
    error_handling("maloc in function separate_str");
  }
  array.len = count;

  char *start = str;
  int index = 0;
  for (char *p = str; *p != '\0'; p++) {
    if (*p == ' ') {
      int length = p - start;
      add_token(&array, index++, start, length);
      start = p + 1;
    }
  }

  if (*start != '\0') {
    add_token(&array, index, start, strlen(start));
  }

  return array;
}

void free_td_array(td_array *array) {
  for (int i = 0; i < array->len; i++) {
    free(array->lines[i]);
  }
  free(array->lines);
}

void remove_element(td_array* array, int index){
  int size = array->len;
  if(index < 0 || index >= size){
    error_handling("Error incorrect index");
    return;
  }

  for(int i = index; i < size - 1; i++){
    array->lines[i] = array->lines[i + 1];
  }

  array->len--;
  array->lines[size] = NULL;
}
