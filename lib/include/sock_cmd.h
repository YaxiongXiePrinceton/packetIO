#ifndef SOCK_CMD_H
#define SOCK_CMD_H
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
  CON_ACK = (char)0x12,     // request to build connection
  CON_CLOSE = (char)0x13,   // request to close the connection
  DATA                      // normal data packet
} sock_cmd_type_t;

sock_cmd_type_t sock_cmd_identify_pkt_type(char buf[4]);
int sock_cmd_generate_pkt_type(char buf[4], sock_cmd_type_t type);

int sock_cmd_print_type(sock_cmd_type_t pkt_type);
#ifdef __cplusplus
}
#endif

#endif
