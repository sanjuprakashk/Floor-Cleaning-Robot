#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "uart.h"

pthread_t heartbeat_thread;

extern pthread_mutex_t lock_res;


#endif