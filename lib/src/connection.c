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
//
int connection_starter(int sock_fd, struct sockaddr_in remote_addr) {
  char recvBuf[1400];

  // send the request command
  printf("CONNECTION Starter: ");
  sock_cmd_sent_w_type(sock_fd, remote_addr, CON_REQUEST);

  struct sockaddr_in serv_addr;
  int recvLen = 0, recvPkt = 0;
  int len = sizeof(serv_addr);
  int64_t curr_t, start_t = timestamp_ms();

  // Waiting for the packets from the starter
  while (true) {
    // recv data from client and get the client address
    recvLen = recvfrom(sock_fd, (char *)recvBuf, 1400, MSG_WAITALL,
                       (struct sockaddr *)&serv_addr, &len);
    if (recvLen > 0) {
      // printf("CONNECTION Starter: PORT: %d recv len:%d | %d %d \n",
      //        serv_addr.sin_port, recvLen, recvBuf[0], recvBuf[1]);
      //  we recevie the connection request from client
      if (sock_cmd_identify_pkt_type(recvBuf) == CON_ACK) {
        printf("CONNECTION Starter: Received CMD-type: CON_ACK\n");
        recvPkt++;
      }
    }
    curr_t = timestamp_ms();
    /* if we already received 2 packets or the timer (200ms) is fired */
    if (recvPkt >= 2 || (curr_t - start_t >= 200)) {
      break;
    }
  }
  printf("CONNECTION Starter: Connection Establish!\n");

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
      // printf("Connection Responder: PORT: %d recv len:%d | %02xd %02xd \n",
      //        cli_addr.sin_port, recvLen, recvBuf[0], recvBuf[1]);
      //  we recevie the connection request from client
      if (sock_cmd_identify_pkt_type(recvBuf) == CON_REQUEST) {
        printf("Connection Responder: Received CMD-type: CON_REQUEST\n");
        recvPkt++;
      }
      curr_t = timestamp_ms();
      /* if we already received 2 packets or the timer (200ms) is fired */
      if (recvPkt >= 2 || (curr_t - start_t >= 200)) {
        break;
      }
    }
  }
  printf("Connection Responder: ");
  if (!sock_cmd_sent_w_type(sock_fd, cli_addr, CON_ACK)) {
    printf("Connection Responder: ERROR: CONNECTION ACK SENT Failed!\n");
  }

  printf("CONNECTION Responder: Connection Establish!\n");
  return cli_addr;
}
