#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#include "uart.h"
#include "logger.h"

char lux[10];
char distance[10];
pthread_t communication_thread;

communication comm_rec;

void *communication_thread_callback();


char *get_lux();

char *get_distance();

#endif