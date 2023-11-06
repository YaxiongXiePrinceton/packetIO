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

#include "log.h"

int log_pkt_header(pkt_header_t header, FILE *fd) {
  fprintf(fd, "%d\t", header.sequence_number);
  fprintf(fd, "%ld\t", header.sent_timestamp);
  fprintf(fd, "%ld\t", header.recv_timestamp);
  // calculate oneway delay
  // uint64_t oneway_ms = (header.recv_timestamp - header.sent_timestamp) /
  // 1000; fprintf(fd, "%ld\t", oneway_ms);
  fprintf(fd, "\n");
  return 0;
}
