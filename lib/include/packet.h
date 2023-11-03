#ifndef PACKET_H
#define PACKET_H
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
/* Currently, the packet has the following types: */
/*   1, data packets
 *   2, ACK packets
 *   3, DCI packets used to deliver DCI
 *   The users are welcome to extend it by defining your own
 *   packet types*/

/* the packer header */
typedef struct {
  uint32_t sequence_number;
  uint64_t sent_timestamp;
  uint64_t recv_timestamp;
  uint8_t pkt_type;
} pkt_header_t;

int packet_generate(char pkt_buf[1500], pkt_header_t *pkt_header, void *payload,
                    int payload_size);

#ifdef __cplusplus
}
#endif
#endif
