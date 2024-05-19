#ifndef PMDR_UTILS
#define PMDR_UTILS

#ifndef SOCKET_PATH
#define SOCKET_PATH "/tmp/pmdr"
#endif

#define BUFFER_SIZE 256

#include <sys/un.h>
#include <sys/socket.h>

typedef struct{
  struct sockaddr_un address;
  int sockfd;
  socklen_t addrlen;
}sockInfo;

typedef struct{
  int len;
  char** lines;
}td_array;

int create_socket();
struct sockaddr_un create_sockaddr();
void error_handling(const char* error_msg);
void send_message(int sockfd, char* buffer);
void recv_message(int sockfd, char* buffer);
td_array separate_str(char* str);
void remove_element(td_array* array, int index);
void free_td_array(td_array *array);
#endif
