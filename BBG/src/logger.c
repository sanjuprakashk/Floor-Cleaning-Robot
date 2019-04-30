/**
 * @\file   logger.c
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the function definitions for the logger
 * @\date   04/29/2019
 *
 */

#include "logger.h"

pthread_mutex_t lock; // Mutex

char time_stam[30];

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
char *time_stamp()
{
	
    memset(time_stam,'\0',30);
	time_t timer;
	timer = time(NULL);
	strftime(time_stam, 26, "%Y-%m-%d %H:%M:%S", localtime(&timer));
	return time_stam;	
}


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
void logger_init()
{
	file_ptr = fopen("CN.log", "a+");
	fprintf(file_ptr,"Queue Init\n\n");
    	fclose(file_ptr);

    if (pthread_mutex_init(&lock, NULL) != 0) 
    { 
        perror("Mutex init failed\n"); 
    }


    struct mq_attr mq_attributes;

    /* Setting the message queue attributes */
    mq_attributes.mq_flags = 0;
    mq_attributes.mq_maxmsg = 10;
    mq_attributes.mq_msgsize = MAX_BUFFER_SIZE;
    mq_attributes.mq_curmsgs = 0;

    msg_queue = mq_open(QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &mq_attributes);
    printf("Return value of queue open = %d\n", msg_queue);
}


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
void *logger_thread_callback()
{
	char buffer[MAX_BUFFER_SIZE];
	
	printf("Inside logger thread\n");
	
	memset(buffer,'\0',sizeof(buffer));
    sprintf(buffer,"DEBUG CN [%s] Logger thread active\n", time_stamp());
    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);

	file_ptr = fopen("CN.log", "a+");
	while(1)
	{

		if(mq_receive(msg_queue, buffer,MAX_BUFFER_SIZE,0) > 0)
	 	{
	 		pthread_mutex_lock(&lock);
	 		fprintf(file_ptr,"%s",buffer);
	 		fflush(file_ptr);
	 		pthread_mutex_unlock(&lock);
	 	}
	}
	fclose(file_ptr);
	mq_close(msg_queue);
    mq_unlink(QUEUE_NAME);
    pthread_cancel(logger_thread); 	
}


