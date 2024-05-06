#define SOCKET_PATH "/tmp/pmdr"

#include <stdio.h>
#include <unistd.h>
#include "server.h"
#include "client.h"

int main(int argc, char** argv)
{
  if(argc == 1){
    if (access(SOCKET_PATH, F_OK) != -1){
      printf("Socet alredy exist\n");
      return 1;
    }
    init_server();
    printf("Server alredy started\n");
    start_server();
    close_server();
  } else {
    create_client();
    start_client(argv + 1);
    close_client();
  }
  return 0;
}
