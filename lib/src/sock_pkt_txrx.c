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

#include "sock.h"
#include "sock_pkt_txrx.h"
int sock_pkt_send_single(int sock_fd, struct sockaddr_in remote_addr, char *pkt,
                         int pkt_size) {
  // Socket
  sendto(sock_fd, pkt, pkt_size, 0, (const struct sockaddr *)&remote_addr,
         sizeof(remote_addr));

  return 0;
}

int sock_pkt_recv_single(int sock_fd, struct sockaddr_in remote_addr,
                         char *pkt) {
  char recvBuf[1500];
  struct sockaddr_in serv_addr;
  unsigned int len = sizeof(serv_addr);

  int n = 0;
  n = recvfrom(sock_fd, (char *)recvBuf, 1400, MSG_WAITALL,
               (struct sockaddr *)&serv_addr, &len);

  if (n > 0) {
    printf("PORT: %d recv len:%d | %d %d \n", serv_addr.sin_port, n, recvBuf[0],
           recvBuf[1]);
    memcpy(pkt, recvBuf, n);
  }

  return n;
}
