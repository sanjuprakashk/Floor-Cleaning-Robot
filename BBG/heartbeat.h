#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "uart.h"
#include <pthread.h>
pthread_t heartbeat_thread;

extern pthread_mutex_t lock_res;

void *heartbeat_thread_callback();

#endif
