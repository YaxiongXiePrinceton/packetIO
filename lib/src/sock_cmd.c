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

#include "sock_cmd.h"

sock_cmd_type_t sock_cmd_identify_pkt_type(char *buf) {
  for (int i = 0; i < 4; i++) {
    printf("%02x", buf[i]);
  }
  printf("\n");
  if (buf[0] == (char)CON_REQUEST && buf[1] == (char)CON_REQUEST &&
      buf[2] == (char)CON_REQUEST && buf[3] == (char)CON_REQUEST) {
    return CON_REQUEST;
  } else if (buf[0] == (char)CON_ACK && buf[1] == (char)CON_ACK &&
             buf[2] == (char)CON_ACK && buf[3] == (char)CON_ACK) {
    return CON_ACK;
  } else if (buf[0] == (char)CON_CLOSE && buf[1] == (char)CON_CLOSE &&
             buf[2] == (char)CON_CLOSE && buf[3] == (char)CON_CLOSE) {
    return CON_CLOSE;
  } else {
    return DATA;
  }
}

int sock_cmd_generate_pkt_type(char buf[4], sock_cmd_type_t type) {
  if (type == CON_REQUEST || type == CON_CLOSE) {
    for (int i = 0; i < 4; i++) {
      buf[i] = (char)type;
      return 1;
    }
  } else {
    printf("ERROR: Unkown or Incorrect Packet Type!\n");
    return -1;
  }
}
int sock_cmd_print_type(sock_cmd_type_t pkt_type) {
  switch (pkt_type) {
  case CON_REQUEST:
    printf("CON REQUEST \n");
    break;
  case CON_CLOSE:
    printf("CON CLOSE \n");
    break;
  case DATA:
    printf("DATA \n");
    break;
  default:
    printf("UNKNOWN TYPE! \n");
    break;
  }
}
