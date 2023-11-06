#ifndef SOCK_H
#define SOCK_H
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

typedef enum {
  CON_REQUEST = (char)0x11, // request to build connection
  CON_CLOSE = (char)0x22,   // request to close the connection
  DATA                      // normal data packet
} sock_cmd_type_t;

sock_cmd_type_t sock_identify_pkt_type(char buf[4]);
int sock_generate_pkt_type(char buf[4], sock_cmd_type_t type);

void sock_setnonblocking(int sockfd);
bool sock_same_sock_addr(struct sockaddr_in *a, struct sockaddr_in *b);
int sock_connectServer_w_config_udp(char serv_IP[40], int serv_port);

int sock_create_udp_socket(char IP[40], int port);
int sock_close_and_notify_udp(int sockfd);
struct sockaddr_in sock_create_serv_addr(char serv_IP[40], int serv_port);

#ifdef __cplusplus
}
#endif
#endif
