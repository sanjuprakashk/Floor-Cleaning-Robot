#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>

#include "server.h"
#include "communication.h"


int main()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr); 

	if(pthread_create(&remote_request_thread, &attr, remote_request_callback, NULL) != 0)
	{
		perror("Remote socket thread creation failed");
	}

	if(pthread_create(&communication_thread, &attr, communication_thread_callback, NULL) != 0)
	{
		perror("Communication thread creation failed");
	}
}