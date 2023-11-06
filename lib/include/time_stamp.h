#ifndef SOCK_TIME_STMP_H
#define SOCK_TIME_STMP_H
#include <errno.h>
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

#ifdef __cplusplus
extern "C" {
#endif

int64_t timestamp_ns();

int64_t timestamp_us();

int64_t timestamp_ms();

#ifdef __cplusplus
}
#endif

#endif
