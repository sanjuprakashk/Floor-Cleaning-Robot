#ifndef LOGGER_H
#define LOGGER_H

#include "uart.h"
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h> // mkdir
#include <mqueue.h>

#define QUEUE_NAME "/msg_queue"
#define MAX_BUFFER_SIZE	200

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

typedef struct logger_struct
{
	char task_name[5];
	uint32_t timeStamp;

	char log[100];
}logger_struct;

pthread_t logger_thread;

FILE *file_ptr;

mqd_t msg_queue; // Message queue descriptor

extern pthread_mutex_t lock_res;
 
void *logger_thread_callback();

void logger_init();
#endif


