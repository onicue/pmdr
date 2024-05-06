#ifndef PMDR_UTILS
#define PMDR_UTILS

#ifndef SOCKET_PATH
#define SOCKET_PATH "/tmp/pmdr"
#endif

#include <sys/un.h>
#include <sys/socket.h>

typedef struct{
  struct sockaddr_un address;
  int sockfd;
  socklen_t addrlen;
}sockInfo;

int create_socket();
struct sockaddr_un create_sockaddr();
void error_handling(const char* error_msg);
void itoa(unsigned value, char* buffer, size_t buffer_size);

#endif
