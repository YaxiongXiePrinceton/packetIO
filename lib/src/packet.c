#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <netinet/in.h>
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

#include "packet.h"

int packet_generate(char pkt_buf[1500], pkt_header_t *pkt_header, void *payload,
                    int payload_size) {
  int pkt_size = 0;
  memcpy(pkt_buf, pkt_header, sizeof(pkt_header_t));
  pkt_size += sizeof(pkt_header_t);
  memcpy(pkt_buf, payload, payload_size);
  pkt_size += payload_size;

  return pkt_size;
}

int packet_decompose(char pkt_buf[1500], int pkt_size, pkt_header_t *pkt_header,
                     char payload_buf[1500]) {
  // copy the header
  int head_size = sizeof(pkt_header_t);
  memcpy(pkt_header, pkt_buf, head_size);

  memcpy(&pkt_buf[head_size], payload_buf, pkt_size - head_size);

  return 0;
}
