#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <poll.h>
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
#include "packet.h"
#include "sock_cmd.h"
#include "sock_pkt_txrx.h"
#include "time_stamp.h"

int sock_pkt_send_single(int sock_fd, struct sockaddr_in remote_addr, char *pkt,
                         int pkt_size) {
  // Socket
  sendto(sock_fd, pkt, pkt_size, 0, (const struct sockaddr *)&remote_addr,
         sizeof(remote_addr));

  return 0;
}

// This function automatically sent packets to the remote according to the
// configurations we passed:
// pkt_num: the number of packets to be sent
// pkt_size: the size of the packets
// pkt_interval: the interval between two packets
int sock_pkt_send_multi_w_config(int sock_fd, struct sockaddr_in remote_addr,
                                 pkt_tx_config_t pkt_config, FILE *fd) {
  int pkt_num = pkt_config.pkt_num;
  int pkt_interval = pkt_config.pkt_interval;
  int pkt_size = pkt_config.pkt_size;
  pkt_header_t pkt_header;
  char pkt_buf[1500];

  // reset the packet header
  memset(&pkt_header, 0, sizeof(pkt_header_t));
  int pkt_sent = 0;

  uint32_t sequence_number;
  uint64_t sent_timestamp;
  uint64_t recv_timestamp;
  uint8_t pkt_type;

  while (pkt_sent < pkt_num) {
    uint64_t start_t = timestamp_us();

    pkt_header.sequence_number = pkt_sent + 1;
    pkt_header.sent_timestamp = start_t;
    packet_generate_wSize(pkt_buf, &pkt_header, pkt_size);
    sock_pkt_send_single(sock_fd, remote_addr, pkt_buf, pkt_size);
    pkt_sent++;
    log_pkt_header(pkt_header, fd);

    while (true) {
      uint64_t curr_t = timestamp_us();
      if (curr_t - start_t >= pkt_interval)
        break;
    }
    /* printf("PKT Sent:%d \n", pkt_sent); */
  }

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
    /* printf("PORT: %d recv len:%d | %d %d \n", serv_addr.sin_port, n,
     * recvBuf[0], */
    /* recvBuf[1]); */
    memcpy(pkt, recvBuf, n);
  }

  return n;
}

int sock_pkt_recv_multi_no_output(int sock_fd, struct sockaddr_in remote_addr,
                                  FILE *fd) {
  char recvBuf[1500];
  /* wait for incoming packet OR expiry of timer */
  struct pollfd poll_fds[1];
  poll_fds[0].fd = sock_fd;
  poll_fds[0].events = POLLIN;

  struct timespec timeout;
  timeout.tv_sec = 1;
  timeout.tv_nsec = 0;

  sock_cmd_type_t pkt_type;
  int recvLen = 0;
  while (true) {
    fflush(NULL);
    // printf("WHILE LOOPING! pkt_rcv:%d nof_pkt:%d\n", _pkt_received,
    // _nof_pkt);
    ppoll(poll_fds, 1, &timeout, NULL);
    if (poll_fds[0].revents & POLLIN) {
      recvLen = sock_pkt_recv_single(sock_fd, remote_addr, recvBuf);
      if (recvLen > 0) {
        pkt_type = sock_cmd_identify_pkt_type(recvBuf);
        if (pkt_type == DATA) {
          pkt_header_t pkt_header;
          packet_extract_header(recvBuf, recvLen, &pkt_header);
          /* printf("Header sequence number: %d timestamp:%ld\n", */
          /* pkt_header.sequence_number, pkt_header.sent_timestamp); */
          pkt_header.recv_timestamp = timestamp_us();
          log_pkt_header(pkt_header, fd);
        } else if (pkt_type == CON_CLOSE) {
          /* we receive the command to close the connection */
          printf("CONNECTION CLOSE received, close the connection!\n");
          printf(" Good Bye!\n");
          break;
        } else {
          continue;
        }
      }
    }
  }
}
