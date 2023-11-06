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

/* generate the packet using the packet header and payload */
int packet_generate(char pkt_buf[1500], pkt_header_t *pkt_header, void *payload,
                    int payload_size) {
  int pkt_size = 0;

  /* Copy the packet header */
  memcpy(pkt_buf, (char *)pkt_header, sizeof(pkt_header_t));
  pkt_size += sizeof(pkt_header_t);

  /* copy the payload */
  memcpy(pkt_buf + pkt_size, (char *)payload, payload_size);
  pkt_size += payload_size;

  return pkt_size;
}

/* generate the packet using the packet header and packet size*/
/* the total packet size (including the packet header) equals the input size*/
int packet_generate_wSize(char pkt_buf[1500], pkt_header_t *pkt_header,
                          int pkt_all_size) {
  int pkt_size = 0;

  /* Copy the packet header */
  memcpy(pkt_buf, (char *)pkt_header, sizeof(pkt_header_t));
  pkt_size += sizeof(pkt_header_t);

  if (pkt_all_size < pkt_size)
    return pkt_size;

  /* random packet payload */
  for (int i = pkt_size; i < pkt_all_size; i++) {
    pkt_buf[i] = 0xBB;
  }
  return pkt_all_size;
}

/* decompose the packet into a packet header and its payload */
int packet_decompose(char pkt_buf[1500], int pkt_size, pkt_header_t *pkt_header,
                     char payload_buf[1500]) {
  // copy the header
  int head_size = sizeof(pkt_header_t);
  memcpy(pkt_header, pkt_buf, head_size);

  memcpy(payload_buf, pkt_buf + head_size, pkt_size - head_size);

  return 0;
}

int packet_extract_header(char pkt_buf[1500], int pkt_size,
                          pkt_header_t *pkt_header) {
  int head_size = sizeof(pkt_header_t);

  if (pkt_size < head_size) {
    printf("ERROR: Packet size is smaller than header!");
    return -1;
  }
  memcpy(pkt_header, pkt_buf, head_size);
  return 1;
}
