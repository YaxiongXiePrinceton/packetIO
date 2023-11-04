#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

/* The configurations for sending packets */
/* send_pkt: Sending pkt or not?  */
/* send_port: Sending port  */
/* pkt_send__int_us:  the packet interval between 2 pkt_s  */
/* pkt_send_num:     the total amount of packets we would like to sent */
typedef struct {
  int send_pkt;
  int send_port;
  int pkt_send_int_us;
  int pkt_send_num;
} pkt_sent_config_t;

/* The configurations for recving packets */
/* recv_pkt: Receive pkt or not?  */
/* recv_port: Recving port */
typedef struct {
  int recv_pkt;
  int recv_port;
} pkt_recv_config_t;

/* The configurations for server and client */
/* NAT_client: is the local device's public IP known? if not, its a NAT client*/
/* local_IP: the IP of the local device */
/* remote_IP: the IP of the remote device */
typedef struct {
  int connect_starter;
  char local_IP[40];
  char remote_IP[40];
  int remote_port;
  int local_port;

  int separate_txrx; // do we use separate socket for transmit and receive?
  int sender;        // are we the sender? yes, 1; no 0

  // the detailed configurations only works if we use separatte txrx
  // otherwise, we use the above configurations to create the socket
  pkt_sent_config_t pkt_sent_config;
  pkt_recv_config_t pkt_recv_config;
} serv_cli_config_t;

int packetIO_read_config(serv_cli_config_t *config, char *path);

#ifdef __cplusplus
}
#endif

#endif
