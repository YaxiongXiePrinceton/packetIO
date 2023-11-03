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

#include "nat_punch.h"

// NAT punch client
// Send packets to the remote
int nat_punch_client(int sock_fd, struct sockaddr_in serv_addr) {
  char buffer[400];
  buffer[0] = (char)0x13;
  buffer[1] = (char)0x13;
  buffer[2] = (char)0x13;
  buffer[3] = (char)0x13;

  // Socket
  sendto(sock_fd, (char *)buffer, 4, 0, (const struct sockaddr *)&serv_addr,
         sizeof(serv_addr));
  sendto(sock_fd, (char *)buffer, 4, 0, (const struct sockaddr *)&serv_addr,
         sizeof(serv_addr));

  return 0;
}

struct sockaddr_in nat_punch_server(int sock_fd) {
  char recvBuf[1400];
  unsigned int len = 0;
  int n;
  struct sockaddr_in cli_addr;
  memset(&cli_addr, 0, sizeof(struct sockaddr_in));
  len = sizeof(cli_addr);
  while (true) {
    // recv data from client and get the client address
    n = recvfrom(sock_fd, (char *)recvBuf, 1400, MSG_WAITALL,
                 (struct sockaddr *)&cli_addr, &len);
    if (n > 0) {
      printf("PORT: %d recv len:%d | %d %d \n", cli_addr.sin_port, n,
             recvBuf[0], recvBuf[1]);
      // we recevie the connection request from client
      if (recvBuf[0] == (char)0x13 && recvBuf[1] == (char)0x13 &&
          recvBuf[2] == (char)0x13 && recvBuf[3] == (char)0x13) {
        return cli_addr;
      }
    }
  }
  return cli_addr;
}
