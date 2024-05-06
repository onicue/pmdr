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

void itoa(unsigned value, char* buffer, size_t buffer_size){
  if (buffer_size < 33){
    return;
  }
  int buf_counter = 0;
  int symbvols = 10;
  for(;symbvols < value; symbvols*=10);

  for(;symbvols > 0; buf_counter++){
    int tmp = value % symbvols;
    buffer[buf_counter] = tmp + '0';
    symbvols /= 10;
    value -= tmp * symbvols;
  }

  buffer[++buf_counter] = '\0';
}
