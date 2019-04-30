/**
 * @\file	heartbeat.h
 * @\author	Sanju Prakash Kannioth
 * @\brief	This files contains the declarations and header files for the heartbeat
 * @\date	04/29/2019
 *
 */
#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <pthread.h>
#include "POSIX_timer.h"

#include "communication.h"
#include "uart.h"
#include "logger.h"

#define TIVA_HEART_BEAT_CHECK_PERIOD (5) //5 seconds


/* Global variables for dead/alive status detection of Tiva and sensors */
int tiva_active, tiva_active_prev;
int distance_active, distance_active_prev, lux_active, lux_active_prev, water_active, water_active_prev;

int tiva_dead, distance_dead, lux_dead, water_dead;


timer_t  timer_id_heartbeat;

pthread_t heartbeat_thread;

extern pthread_mutex_t lock_res; // Mutex 


/*
--------------------------------------------------------------------------------------------
heartbeat_thread_callback
--------------------------------------------------------------------------------------------
*	This is the thread creation callback function for the heartbeat thread
*
* 	@\param			void
*
* 	@\return		void
*
*/
void *heartbeat_thread_callback();


/*
--------------------------------------------------------------------------------------------
beat_timer_handler
--------------------------------------------------------------------------------------------
*	This function is the timer handler for tiva heart beat timer
*
* 	@\param			signal value ( dummy)
*
* 	@\return		none
*
*/
void beat_timer_handler(union sigval val);

#endif
