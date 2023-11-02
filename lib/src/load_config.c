#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libconfig.h>

#include "load_config.h"

int compar(const void* a,const void* b)
{
    return (*(long long*)a - *(long long*)b);
}

bool containsDuplicate(long long* nums, int numsSize){
    int i,j;
    qsort(nums, numsSize,sizeof(long long),compar);
    for(i = 0,j = 1;j < numsSize;i++,j++)
    {
        if(nums[i] == nums[j])
        {
            return true;
        }
    }
    return false;
}

int packetIO_read_config(serv_cli_config_t* config, char * path)
{
  config_t* cfg = (config_t *)malloc(sizeof(config_t));
  config_init(cfg);

  if(! config_read_file(cfg, path)){
      fprintf(stderr, "%s:%d - %s\n", config_error_file(cfg),
              config_error_line(cfg), config_error_text(cfg));
      config_destroy(cfg);
      return(EXIT_FAILURE);
  }

/********************** IP related *****************************/
  if(! config_lookup_bool(cfg, "IP_known", &config->IP_known)){
      printf("ERROR: reading IP_known\n");
  }
  printf("CONFIG -> IP_known:%d\n", config->IP_known);

  /* Empty the local and remote IP address*/
  memset(config->remote_IP, '\0', sizeof(config->remote_IP));
  memset(config->local_IP, '\0', sizeof(config->local_IP));

  /* read the local IP */
  const char* loca_IP;
  if(! config_lookup_string(cfg, "local_IP", &loca_IP)){
    printf("ERROR: reading local_IP\n");
  }
  strcpy(config->local_IP, loca_IP);

  /* read the remote IP */
  const char* remote_IP;
  if(! config_lookup_string(cfg, "remote_IP", &remote_IP)){
    printf("ERROR: reading remote_IP\n");
  }
  strcpy(config->remote_IP, remote_IP);

/********************** END of IP related ***********************/


/****************** Configurations of Sending Packets *****************/

  /* Read wheather the local device send packet or not */
  if(! config_lookup_bool(cfg, "send_pkt", &config->pkt_sent_config.send_pkt)){
      printf("ERROR: reading send_pkt\n");
  }
  printf("CONFIG -> send_pkt: %d\n", config->pkt_sent_config.send_pkt);

  if(! config_lookup_int(cfg, "send_port", &config->pkt_sent_config.send_port)){
      printf("ERROR: reading send_port\n");
  }
  printf("CONFIG -> send_port: %d\n", config->pkt_sent_config.send_port);
  
  if(! config_lookup_int(cfg, "pkt_send_int_us", &config->pkt_sent_config.pkt_send_int_us)){
      printf("ERROR: reading pkt_send_int_us\n");
  }
  printf("CONFIG -> pkt_send_int_us: %d\n", config->pkt_sent_config.pkt_send_int_us);

  if(! config_lookup_int(cfg, "pkt_send_num", &config->pkt_sent_config.pkt_send_num)){
      printf("ERROR: reading pkt_send_num\n");
  }
  printf("CONFIG -> pkt_send_num: %d\n", config->pkt_sent_config.pkt_send_num);
  /****************** END of Configurations of Sending Packets *****************/


  /****************** Configurations of receiving Packets *****************/
  if(! config_lookup_bool(cfg, "recv_pkt", &config->pkt_recv_config.recv_pkt)){
      printf("ERROR: reading recv_pkt\n");
  }
  printf("CONFIG -> recv_pkt: %d\n", config->pkt_recv_config.recv_pkt);

  if(! config_lookup_int(cfg, "recv_port", &config->pkt_recv_config.recv_port)){
      printf("ERROR: reading recv_port\n");
  }
  printf("CONFIG -> recv_port: %d\n", config->pkt_recv_config.recv_port);

  /****************** Configurations of receiving Packets *****************/

  return 0;
}
