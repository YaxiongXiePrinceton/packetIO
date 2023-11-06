#ifndef SOCK_PKT_TXRX_H
#define SOCK_PKT_TXRX_H
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

int sock_pkt_send_single(int sock_fd, struct sockaddr_in remote_addr, char *pkt,
                         int pkt_size);

#ifdef __cplusplus
}
#endif

#endif
