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

#include "load_config.h"
#include "sock.h"

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

  int sock_fd[2] = {0}; // the first socket is for recv and 2nd for send

  if (pkt_recv)
    sock_fd[0] = sock_create_udp_socket(config.local_IP,
                                        config.pkt_recv_config.recv_port);
  if (send_port != recv_port && pkt_send)
    sock_fd[1] = sock_create_udp_socket(config.local_IP,
                                        config.pkt_sent_config.send_port);
  // if our current IP is known
  if (config.IP_known) {
    // We should be the NAT server
  } else {
    // We should be the NAT client
  }

  return 0;
}
