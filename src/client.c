#include "utils.h"
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

sockInfo client;

void create_client(){
  client.sockfd = create_socket();
  client.address = create_sockaddr();
}

void close_client(){
  close(client.sockfd);
}


void start_client(int argc, char** argv){
  char buffer[BUFFER_SIZE] = "";
  for(int i = 1; i < argc; i++){
      strcat(buffer, argv[i]);
      if(i < argc - 1){
          strcat(buffer, " ");
      }
  }
  if(connect(client.sockfd, (struct sockaddr *)&client.address, sizeof(client.address)) < 0){
      error_handling("while connecting to server");
  }
  send_message(client.sockfd, buffer);
  recv_message(client.sockfd, buffer);
  if(strlen(buffer)){
    printf("%s", buffer);
  }
}
