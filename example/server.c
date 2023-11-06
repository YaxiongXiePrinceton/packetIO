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

#include "connection.h"
#include "load_config.h"
#include "nat_punch.h"
#include "packet.h"
#include "sock.h"
#include "sock_pkt_txrx.h"

int main(int argc, char **argv) {
  int c;
  serv_cli_config_t config;
  char *config_path = NULL;

  /* Parsing command line arguments */
  while ((c = getopt(argc, argv, "c:s:o:h")) != -1) {
    switch (c) {
    case 'c':
      config_path = optarg;
      break;
    case 'h':
      return 0;
    case '?':
      if (optopt == 'c')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else {
        return 1;
      }
    default:
      return 1;
    }
  }

  packetIO_read_config(&config, config_path);

  bool separate_txrx = config.separate_txrx;
  bool pkt_send = config.pkt_sent_config.send_pkt;
  bool send_port = config.pkt_sent_config.send_port;
  bool pkt_recv = config.pkt_recv_config.recv_pkt;
  bool recv_port = config.pkt_recv_config.recv_port;

  int sock_fd = 0;
  sock_fd = sock_create_udp_socket(config.local_IP, config.local_port);

  struct sockaddr_in remote_addr;
  // now we need to figure out the remote addr
  if (config.connect_starter) {
    remote_addr = sock_create_serv_addr(config.remote_IP, config.remote_port);
    connection_starter(sock_fd, remote_addr);
  } else {
    remote_addr = connection_responder(sock_fd);
  }

  /* pkt_header_t pkt_header; */
  /* memset(&pkt_header, 0, sizeof(pkt_header_t)); */
  /**/
  /* pkt_header.sequence_number = 1010; */
  /* char pkt_buf[1500]; */
  /* int pkt_size; */
  /**/
  /* printf("header size:%d payload_size:%d \n", sizeof(pkt_header_t), */
  /*        sizeof(serv_cli_config_t)); */
  /* pkt_size = packet_generate(pkt_buf, &pkt_header, (void *)&config, */
  /*                            sizeof(serv_cli_config_t)); */
  /**/
  /* pkt_header_t new_pkt_header; */
  /**/
  /* char payload_buf[1500]; */
  /* packet_decompose(pkt_buf, pkt_size, &new_pkt_header, payload_buf); */
  /* printf("decompese done! header_seq:%d\n", new_pkt_header.sequence_number);
   */
  /**/
  /* serv_cli_config_t new_config; */
  /* memcpy(&new_config, payload_buf, sizeof(serv_cli_config_t)); */
  /**/
  /* printf("config knownIP:%d local IP:%s separateTXRX:%d\n",
   * new_config.IP_known, */
  /*        new_config.local_IP, new_config.separate_txrx); */
  /**/

  if (config.sender) {
    // sender
    pkt_header_t pkt_header;
    memset(&pkt_header, 0, sizeof(pkt_header_t));
    pkt_header.sequence_number = 1010;
    char pkt_buf[1500];
    int pkt_size;
    pkt_size = packet_generate(pkt_buf, &pkt_header, (void *)&config,
                               sizeof(serv_cli_config_t));

    sock_pkt_send_single(sock_fd, remote_addr, pkt_buf, pkt_size);
  } else {
    // receiver
    char pkt_buf[1500];
    int pkt_size = 0;
    pkt_size = sock_pkt_recv_single(sock_fd, remote_addr, pkt_buf);
    pkt_header_t pkt_header;
    char payload_buf[1500];
    packet_decompose(pkt_buf, pkt_size, &pkt_header, payload_buf);
    serv_cli_config_t new_config;
    memcpy(&new_config, payload_buf, sizeof(serv_cli_config_t));
    printf("config connect_starter:%d \n", new_config.connect_starter);
  }

  return 0;
}
