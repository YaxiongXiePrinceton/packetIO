#ifndef NAT_PUNCH_H
#define NAT_PUNCH_H
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

int connection_starter(int sock_fd, struct sockaddr_in serv_addr);
struct sockaddr_in connection_responder(int sock_fd);

#ifdef __cplusplus
}
#endif

#endif
