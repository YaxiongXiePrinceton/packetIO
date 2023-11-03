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

int main(int argc, char** argv){
  int c;
  serv_cli_config_t config;
  char* config_path = NULL;

  /* Parsing command line arguments */
  while ((c = getopt (argc, argv, "c:s:o:h")) != -1) {
    switch (c) {
      case 'c':
        config_path = optarg;
        break;
      case 'h':
        return 0;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else {
          return 1;
        }
      default:
        return 1;
    }
  }
  
  packetIO_read_config(&config, config_path);

  int sock_fd;
  struct sockaddr_in servaddr;

  if(config.IP_known){
    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }

    // non-blocking
    sock_setnonblocking(sockfd);
   
    // Set server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket to server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
    } 
  }else{

  }
}
