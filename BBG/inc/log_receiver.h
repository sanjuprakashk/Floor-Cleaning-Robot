/**
 * @\file	log_receiver.h
 * @\author	Sanju Prakash Kannioth and Steve Antony X
 * @\brief	This files contains the declarations and header files for tiva log receiver module
 * @\date	03/30/2019
 *
 */


#ifndef LOG_RECEIVER_H
#define LOG_RECEIVER_H

#include "uart.h"
#include "logger.h"

pthread_t log_receiver_thread;


/**
--------------------------------------------------------------------------------------------
revecive_thread_callback
--------------------------------------------------------------------------------------------
*   This function is the thread callback function for logger messages coming from Tiva
*
*   @\param         void
*
*   @\return        void
*
*/
void *revecive_thread_callback();

#endif