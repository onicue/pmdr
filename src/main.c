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
  int ac = access(SOCKET_PATH, F_OK);

  if(argc == 1 ||
    (argc >= 2 && argc < 5 && !strcmp(argv[1], "start")) && ac!=0)
  {
    if (ac == 0)
      error_handling("socket alredy exist");
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
      error_handling("signal handler installation failed");
    init_server();
    if (argc > 1){
      start_reverse_timer(argc - 1, argv+1);
    }
    printf("Server alredy started\n");
    start_server();
    close_server();
  } else {
    if (ac == -1)
      error_handling("socket don't exist");
    create_client();
    start_client( argc, argv);
    close_client();
  }
  return 0;
}
