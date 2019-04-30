#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "uart.h"
#include <pthread.h>
#include "POSIX_timer.h"
#include "communication.h"

#define TIVA_HEART_BEAT_CHECK_PERIOD (5)//5 s

int tiva_active, tiva_active_prev;
int distance_active, distance_active_prev, lux_active, lux_active_prev, water_active, water_active_prev;

int tiva_dead, distance_dead, lux_dead, water_dead;


timer_t  timer_id_heartbeat;


pthread_t heartbeat_thread;

extern pthread_mutex_t lock_res;

void *heartbeat_thread_callback();

void beat_timer_handler(union sigval val);

#endif
