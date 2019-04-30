/**
 * @\file	communication.h
 * @\author	Sanju Prakash Kannioth
 * @\brief	This files contains the declarations and header files for the communication interface for tiva and BBG
 * @\date	04/29/2019
 *
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <pthread.h>
#include <stdint.h>

#include "uart.h"
#include "logger.h"
#include "POSIX_timer.h"
#include "heartbeat.h"


/* 
*	GLOBALS
*/
char lux[10];
char distance[10];
char waterLevel[10];
char mode[10];
char dg_mode[10];
char tiva_opstatus[10];
char distance_opstatus[10];
char lux_opstatus[10];
char water_opstatus[10];
char valve_status[10];

pthread_t communication_thread;

uint8_t already_open;
uint8_t already_closed;
uint8_t water_outOfRange;

extern pthread_mutex_t lock_res;


/*
--------------------------------------------------------------------------------------------
communication_thread_callback
--------------------------------------------------------------------------------------------
*	This is the thread creation callback function for the communication thread
*
* 	@\param			void
*
* 	@\return		void
*
*/
void *communication_thread_callback();


/*
--------------------------------------------------------------------------------------------
get_lux
--------------------------------------------------------------------------------------------
*	This function returns the most updated lux value in string format
*
* 	@\param			void
*
* 	@\return		string containing most recent lux value
*
*/
char *get_lux();


/*
--------------------------------------------------------------------------------------------
get_distance
--------------------------------------------------------------------------------------------
*	This function returns the most updated distance value in string format
*
* 	@\param			void
*
* 	@\return		string containing most recent distance value
*
*/
char *get_distance();


/*
--------------------------------------------------------------------------------------------
get_waterLevel
--------------------------------------------------------------------------------------------
*	This function returns the most updated water level value in string format
*
* 	@\param			void
*
* 	@\return		string containing most recent water level value
*
*/
char *get_waterLevel();


/*
--------------------------------------------------------------------------------------------
get_mode
--------------------------------------------------------------------------------------------
*	This function returns the most updated operating mode in string format
*
* 	@\param			void
*
* 	@\return		string containing most updated operating mode value
*
*/
char *get_mode();


/*
--------------------------------------------------------------------------------------------
get_dgMode
--------------------------------------------------------------------------------------------
*	This function returns if the system is in degraded mode in string format
*
* 	@\param			void
*
* 	@\return		string containing degraded mode value
*
*/
char *get_dgMode();


/*
--------------------------------------------------------------------------------------------
get_opStatus_tiva
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the remote node
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the remote node
*
*/
char *get_opStatus_tiva();


/*
--------------------------------------------------------------------------------------------
get_opStatus_distance
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the distance sensor
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the distance sensor
*
*/
char *get_opStatus_distance();


/*
--------------------------------------------------------------------------------------------
get_opStatus_lux
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the lux sensor
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the lux sensor
*
*/
char *get_opStatus_lux();


/*
--------------------------------------------------------------------------------------------
get_opStatus_water
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the water level sensor
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the water level sensor
*
*/
char *get_opStatus_water();


/*
--------------------------------------------------------------------------------------------
get_valveStatus
--------------------------------------------------------------------------------------------
*	This function returns the most updated status of the valve
*
* 	@\param			void
*
* 	@\return		string containing the most updated status of the valve
*
*/
char *get_valveStatus();
#endif
