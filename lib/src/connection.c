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

#include "connection.h"
#include "sock_cmd.h"
#include "time_stamp.h"
// Send packets to the remote
int connection_starter(int sock_fd, struct sockaddr_in serv_addr) {
  char recvBuf[1400];

  sock_cmd_generate_pkt_type(recvBuf, CON_REQUEST);

  printf("STARTER: tx buf %d %d\n", recvBuf[0], recvBuf[1]);

  sendto(sock_fd, (char *)recvBuf, 4, 0, (const struct sockaddr *)&serv_addr,
         sizeof(serv_addr));
  sendto(sock_fd, (char *)recvBuf, 4, 0, (const struct sockaddr *)&serv_addr,
         sizeof(serv_addr));

  int recvLen = 0, recvPkt = 0;
  int len = sizeof(serv_addr);
  int64_t curr_t, start_t = timestamp_ms();

  // Waiting for the packets from the starter
  while (true) {
    // recv data from client and get the client address
    recvLen = recvfrom(sock_fd, (char *)recvBuf, 1400, MSG_WAITALL,
                       (struct sockaddr *)&serv_addr, &len);
    if (recvLen > 0) {
      printf("CONNECTION Starter: PORT: %d recv len:%d | %d %d \n",
             serv_addr.sin_port, recvLen, recvBuf[0], recvBuf[1]);
      // we recevie the connection request from client
      if (sock_cmd_identify_pkt_type(recvBuf) == CON_ACK) {
        recvPkt++;
      }
    }
    curr_t = timestamp_ms();
    /* if we already received 2 packets or the timer (200ms) is fired */
    if (recvPkt >= 2 || (curr_t - start_t >= 200)) {
      break;
    }
  }

  return 0;
}
// The responder
struct sockaddr_in connection_responder(int sock_fd) {
  char recvBuf[1400];
  unsigned int len = 0;
  int n;
  struct sockaddr_in cli_addr;
  memset(&cli_addr, 0, sizeof(struct sockaddr_in));
  len = sizeof(cli_addr);

  int recvLen = 0, recvPkt = 0;
  int64_t curr_t, start_t = timestamp_ms();
  // Waiting for the packets from the starter
  while (true) {
    // recv data from client and get the client address
    recvLen = recvfrom(sock_fd, (char *)recvBuf, 1400, MSG_WAITALL,
                       (struct sockaddr *)&cli_addr, &len);
    if (recvLen > 0) {
      printf("Connection Responder: PORT: %d recv len:%d | %d %d \n",
             cli_addr.sin_port, recvLen, recvBuf[0], recvBuf[1]);
      // we recevie the connection request from client
      if (sock_cmd_identify_pkt_type(recvBuf) == CON_REQUEST) {
        recvPkt++;
      }
      curr_t = timestamp_ms();
      /* if we already received 2 packets or the timer (200ms) is fired */
      if (recvPkt >= 2 || (curr_t - start_t >= 200)) {
        break;
      }
    }
  }

  // Generate and Send acks back to the starter
  sock_cmd_generate_pkt_type(recvBuf, CON_ACK);
  printf("Responder: tx buf %d %d\n", recvBuf[0], recvBuf[1]);
  sendto(sock_fd, (char *)recvBuf, 4, 0, (const struct sockaddr *)&cli_addr,
         sizeof(cli_addr));
  sendto(sock_fd, (char *)recvBuf, 4, 0, (const struct sockaddr *)&cli_addr,
         sizeof(cli_addr));

  return cli_addr;
}
