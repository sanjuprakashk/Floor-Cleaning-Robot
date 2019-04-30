/**
 * @\file   logger.h
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the declarations and header files for the logger
 * @\date   04/29/2019
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "uart.h"
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h> // mkdir
#include <mqueue.h>

#define QUEUE_NAME "/msg_queue" // Message queue name
#define MAX_BUFFER_SIZE	200     // Message queue max buffer size


/* Sensor structure that is sent from Tiva */
typedef struct sensor_struct
{
    char task_name[5];
    uint32_t timeStamp;

    float distance;
    float lux;
    uint32_t water;
    int8_t mode;
    int8_t dg_mode;
}sensor_struct;

/* Logger structure that is sent from Tiva */
typedef struct logger_struct
{
	char task_name[5];
	uint32_t timeStamp;

	char log[100];
}logger_struct;

pthread_t logger_thread;

FILE *file_ptr;

mqd_t msg_queue; // Message queue descriptor

extern pthread_mutex_t lock_res; // Mutex
 

/**
--------------------------------------------------------------------------------------------
time_stamp
--------------------------------------------------------------------------------------------
*   This function will format the timestamp
*
*   @\param         
*
*   @\return        timestamp as a string
*
*/
char *time_stamp();


/**
--------------------------------------------------------------------------------------------
logger_thread_callback
--------------------------------------------------------------------------------------------
*   This function is the thread callback function for the logger
*
*   @\param         void
*
*   @\return        void
*
*/
void *logger_thread_callback();


/**
--------------------------------------------------------------------------------------------
logger_init
--------------------------------------------------------------------------------------------
*   This function will initialize the logger
*
*   @\param         void
*
*   @\return        void
*
*/
void logger_init();
#endif


