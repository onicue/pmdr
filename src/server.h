#ifndef PMDR_SERVER
#define PMDR_SERVER
#include <sys/un.h>

#ifndef DEFAULT_WORK_TIME
#define DEFAULT_WORK_TIME 1500
#endif

#ifndef DEFAULT_REST_TIME
#define DEFAULT_REST_TIME 300
#endif

void init_server();
void start_server();
void close_server();
void start_reverse_timer(int argc, char** argv);
#endif
