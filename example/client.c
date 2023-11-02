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
  pkt_recv_config_t config;
  char* config_path = NULL;

  /* Parsing command line arguments */
  while ((c = getopt (argc, argv, "c:s:o:h")) != -1) {
    switch (c) {
      case 'c':
        config_path = optarg;
        break;
      case 'h':
        print_help();
        return 0;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else {
          print_help();
          return 1;
        }
      default:
        print_help();
        return 1;
    }
  }
  
  packetIO_read_config(&config, config_path);

}
