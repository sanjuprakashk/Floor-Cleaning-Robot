#include "logger.h"

pthread_mutex_t lock;

/* Function to initialize the logger */
void logger_init()
{
	file_ptr = fopen("test.txt", "w+");
	fprintf(file_ptr,"Queue Init\n\n");
    	fclose(file_ptr);

    if (pthread_mutex_init(&lock, NULL) != 0) 
    { 
        perror("Mutex init failed\n"); 
    }


    struct mq_attr mq_attributes;

    /* Setting the message queue attributes */
    mq_attributes.mq_flags = 0;
    mq_attributes.mq_maxmsg = 5;
    mq_attributes.mq_msgsize = MAX_BUFFER_SIZE;
    mq_attributes.mq_curmsgs = 0;

    msg_queue = mq_open(QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &mq_attributes);
    printf("Return value of queue open = %d\n", msg_queue);
}

void *logger_thread_callback()
{
	char buffer[MAX_BUFFER_SIZE];
	printf("Inside logger thread\n");
	
	
	file_ptr = fopen("/home/debian/Floor-Cleaning-Robot/BBG/test.txt", "a+");
	while(1)
	{

		if(mq_receive(msg_queue, buffer, MAX_BUFFER_SIZE, 0) > 0)
	 	{
	 		pthread_mutex_lock(&lock);
	 		fprintf(file_ptr,"%s",buffer);
	 		fflush(file_ptr);
	 		pthread_mutex_unlock(&lock);
	 	}
	}	

}


