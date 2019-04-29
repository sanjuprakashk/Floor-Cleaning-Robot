#ifndef LOG_RECEIVER_H
#define LOG_RECEIVER_H

#include "uart.h"
#include "logger.h"

pthread_t log_receiver_thread;


void *revecive_thread_callback();

#endif