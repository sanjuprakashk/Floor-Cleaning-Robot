#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#include "uart.h"

pthread_t communication_thread;

communication comm_rec;

void *communication_thread_callback();


float get_lux();

float get_distance();

#endif