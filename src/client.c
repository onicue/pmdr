#include "utils.h"
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>

sockInfo client;

void create_client(){
  client.sockfd = create_socket();
  client.address = create_sockaddr();
}

void close_client(){
  close(client.sockfd);
}

void start_client(char** argv){
  char buffer[256];
  if(connect(client.sockfd, (struct sockaddr *)&client.address, sizeof(client.address)) < 0){
    error_handling("Error while connecting to server");
  }

  if(send(client.sockfd, argv[0], strlen(argv[0]), 0) < 0){
    error_handling("Error while sending message");
  }

  int bytes_received = recv(client.sockfd, buffer, sizeof(buffer),0);
  if(bytes_received < 0){
    error_handling("Error while receiving data");
  }
  buffer[bytes_received] = '\0';
  printf("%s\n", buffer);
}
