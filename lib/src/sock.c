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
// set sock in non-block mode
void sock_setnonblocking(int sockfd) {
  int flag = fcntl(sockfd, F_GETFL, 0);
  if (flag < 0) {
    perror("fcntl F_GETFL fail");
    return;
  }

  if (fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) < 0) {
    perror("fcntl F_SETFL fail");
  }
  return;
}

sock_cmd_type_t sock_identify_pkt_type(char *buf) {
  if (buf[0] == (char)CON_REQUEST && buf[1] == (char)CON_REQUEST &&
      buf[2] == (char)CON_REQUEST && buf[3] == (char)CON_REQUEST) {
    return CON_REQUEST;
  } else if (buf[0] == (char)CON_CLOSE && buf[1] == (char)CON_CLOSE &&
             buf[2] == (char)CON_CLOSE && buf[3] == (char)CON_CLOSE) {
    return CON_CLOSE;
  } else {
    return DATA;
  }
}

int sock_generate_pkt_type(char buf[4], sock_cmd_type_t type) {
  if (type == CON_REQUEST || type == CON_CLOSE) {
    for (int i = 0; i < 4; i++) {
      buf[i] = (char)type;
      return 1;
    }
  } else {
    printf("ERROR: Unkown or Incorrect Packet Type!\n");
    return -1;
  }
}
// check whether two socks are the same
bool sock_same_sock_addr(struct sockaddr_in *a, struct sockaddr_in *b) {
  if (a->sin_family == b->sin_family && a->sin_port == b->sin_port) {
    if (a->sin_addr.s_addr == b->sin_addr.s_addr) {
      bool match = true;
      for (int i = 0; i < 8; i++) {
        if (a->sin_zero[i] != b->sin_zero[i]) {
          match = false;
        }
      }
      if (match)
        return true;
    }
  }
  return false;
}

/* Create a UDP socket with IP and port */
/* if the port is zero, then we didn't bind the socket to any IP and port */
int sock_create_udp_socket(char IP[40], int port) {
  int sockfd;
  struct sockaddr_in servaddr;

  // Create UDP socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // sock_setnonblocking(sockfd);

  printf("IP is:%s\n", IP);

  /* sleep(2); */
  if (port > 0) {
    // Set IP address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    /* if the IP address is not empty */
    if (strlen(IP) > 3) {
      // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
      if (inet_pton(AF_INET, IP, &(servaddr.sin_addr)) <= 0) {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
      }
    } else {
      /* any IP are valid -- equals to 0.0.0.0*/
      servaddr.sin_addr.s_addr = INADDR_ANY;
    }
    // Bind socket to server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) <
        0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
  }
  return sockfd;
}

int sock_connectServer_w_config_udp(char serv_IP[40], int serv_port) {
  int sockfd;
  char buffer[1400];
  struct sockaddr_in servaddr;

  // Create UDP socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  sock_setnonblocking(sockfd);

  // Set server address
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(serv_port);
  // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
  //
  if (inet_pton(AF_INET, serv_IP, &(servaddr.sin_addr)) <= 0) {
    perror("Invalid server IP address");
    exit(EXIT_FAILURE);
  }

  buffer[0] = (char)0xCC;
  buffer[1] = (char)0xCC;
  buffer[2] = (char)0xCC;
  buffer[3] = (char)0xCC;

  // Socket
  sendto(sockfd, (char *)buffer, 4, 0, (const struct sockaddr *)&servaddr,
         sizeof(servaddr));
  sendto(sockfd, (char *)buffer, 4, 0, (const struct sockaddr *)&servaddr,
         sizeof(servaddr));

  int nof_serv_pkt = 0;
  while (true) {
    unsigned int len = sizeof(servaddr);
    int recvLen = recvfrom(sockfd, (char *)buffer, 1400, 0,
                           (struct sockaddr *)&servaddr, &len);
    if (recvLen > 0) {
      if (buffer[0] == (char)0xAA && buffer[1] == (char)0xAA &&
          buffer[2] == (char)0xAA && buffer[3] == (char)0xAA) {
        nof_serv_pkt++;
      }
      if (nof_serv_pkt >= 2) {
        break;
      }
    }
    usleep(100000);
  }

  return sockfd;
}

// close the connection with the server
// notify the server about it
int sock_close_and_notify_udp(int sockfd) {
  char buffer[1400];
  struct sockaddr_in servaddr;
  int PORT = 6767;

  // Set server address
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器IP地址

  buffer[0] = (char)0xFF;
  buffer[1] = (char)0xFF;
  buffer[2] = (char)0xFF;
  buffer[3] = (char)0xFF;

  sendto(sockfd, (char *)buffer, 4, 0, (const struct sockaddr *)&servaddr,
         sizeof(servaddr));
  sendto(sockfd, (char *)buffer, 4, 0, (const struct sockaddr *)&servaddr,
         sizeof(servaddr));
  sendto(sockfd, (char *)buffer, 4, 0, (const struct sockaddr *)&servaddr,
         sizeof(servaddr));

  return 0;
}

struct sockaddr_in sock_create_serv_addr(char serv_IP[40], int serv_port) {
  struct sockaddr_in servaddr;

  // Set server address
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(serv_port);
  // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
  if (inet_pton(AF_INET, serv_IP, &(servaddr.sin_addr)) <= 0) {
    perror("Invalid server IP address");
    exit(EXIT_FAILURE);
  }

  return servaddr;
}
