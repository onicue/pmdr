#define SOCKET_PATH "/tmp/pmdr" //this line can be delete

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"
#include "utils.h"

void sigint_handler(int signo){
  close_server();
  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv){
  if(argc == 1){
    if (access(SOCKET_PATH, F_OK) == 0)
      error_handling("socket alredy exist");
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
      error_handling("signal handler installation failed");
    init_server();
    printf("Server alredy started\n");
    start_server();
    close_server();
  } else {
    if (access(SOCKET_PATH, F_OK) == -1)
      error_handling("socket alredy exist");
    create_client();
    start_client( argc, argv);
    close_client();
  }
  return 0;
}
