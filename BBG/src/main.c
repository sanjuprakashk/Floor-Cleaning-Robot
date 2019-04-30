#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>

#include "server.h"
#include "communication.h"
#include "logger.h"
#include "heartbeat.h"
#include "log_receiver.h"


pthread_mutex_t lock_res; // Mutex 

int main()
{
	char buffer[MAX_BUFFER_SIZE];

	pthread_attr_t attr;
	pthread_attr_init(&attr); 

	logger_init();

	/* Create logger thread */
	if(pthread_create(&logger_thread, &attr, logger_thread_callback, NULL) != 0)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Logger thread creation failed\n",time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Logger thread creation failed");
	}

	if (pthread_mutex_init(&lock_res, NULL) != 0) 
    { 
        perror("Mutex init failed\n"); 
        memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Mutex init failed\n",time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
        return -1; 
    }

	if(pthread_create(&remote_request_thread, &attr, remote_request_callback, NULL) != 0)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Remote request thread creation failed\n",time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Remote socket thread creation failed");
	}

	if(pthread_create(&communication_thread, &attr, communication_thread_callback, NULL) != 0)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Communication thread creation failed\n",time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Communication thread creation failed");
	}

	if(pthread_create(&log_receiver_thread, &attr, revecive_thread_callback, NULL) != 0)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Logger receive thread creation failed\n",time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Receiver logger thread creation failed");
	}

	if(pthread_create(&heartbeat_thread, &attr, heartbeat_thread_callback, NULL) != 0)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Heartbeat thread creation failed\n",time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Heartbeat thread creation failed");
	}

	memset(buffer,'\0',sizeof(buffer));
    sprintf(buffer,"DEBUG CN [%s] Threads creation success\n",time_stamp());
    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	
	printf("Threads created successfully\n");

	pthread_join(communication_thread,NULL);
	pthread_join(logger_thread,NULL);
	pthread_join(log_receiver_thread,NULL);	
	pthread_join(heartbeat_thread, NULL);
	pthread_join(remote_request_thread,NULL);

	return 0;
}
