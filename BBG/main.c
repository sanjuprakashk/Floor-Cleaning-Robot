#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>

#include "server.h"
#include "communication.h"
#include "logger.h"
#include "heartbeat.h"

pthread_mutex_t lock_res;

int main()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr); 

	if (pthread_mutex_init(&lock_res, NULL) != 0) 
    { 
        perror("Mutex init failed\n"); 
        return -1; 
    }

	if(pthread_create(&remote_request_thread, &attr, remote_request_callback, NULL) != 0)
	{
		perror("Remote socket thread creation failed");
	}

	if(pthread_create(&communication_thread, &attr, communication_thread_callback, NULL) != 0)
	{
		perror("Communication thread creation failed");
	}

	if(pthread_create(&logger_thread, &attr, logger_thread_callback, NULL) != 0)
	{
		perror("Logger thread creation failed");
	}

	if(pthread_create(&heartbeat_thread, &attr, heartbeat_thread_callback, NULL) != 0)
	{
		perror("Heartbeat thread creation failed");
	}

	printf("Threads created successfully\n");

	
	pthread_join(communication_thread,NULL);
	pthread_join(logger_thread,NULL);
	pthread_join(heartbeat_thread, NULL);
	pthread_join(remote_request_thread,NULL);

	return 0;
}
