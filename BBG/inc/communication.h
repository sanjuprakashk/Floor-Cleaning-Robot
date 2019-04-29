#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <pthread.h>
#include <stdint.h>

#include "uart.h"
#include "logger.h"
#include "POSIX_timer.h"
#include "heartbeat.h"

char lux[10];
char distance[10];
char waterLevel[10];
char mode[10];
char dg_mode[10];

pthread_t communication_thread;

uint8_t already_open;
uint8_t already_closed;

extern pthread_mutex_t lock_res;


void *communication_thread_callback();


char *get_lux();

char *get_distance();

char *get_waterLevel();

char *get_mode();

char *get_dgMode();
#endif
