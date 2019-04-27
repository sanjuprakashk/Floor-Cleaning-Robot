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
    mq_attributes.mq_maxmsg = 100;
    mq_attributes.mq_msgsize = MAX_BUFFER_SIZE;
    mq_attributes.mq_curmsgs = 100;

    msg_queue = mq_open(QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &mq_attributes);
}
void *logger_thread_callback()
{
	char buffer[MAX_BUFFER_SIZE];
	printf("Inside logger thread");
	uart_properties *uart1  = malloc(sizeof(uart_properties));
	uart1->uart_no = 1;
	uart1->baudrate = B115200;

	uint8_t isOpen1 = uart_config(uart1);
	
	char log[15];
	//char b = 's';
	
	while(1)
	{
		//uart_send(uart1,&b,sizeof(char));

		// usleep(75000);
		memset(log,'\0', sizeof(log));
		if(uart_receive_task(uart1,&log,sizeof(log)) > 0)
		{
		//	printf("UART1\n");
		}

		if(mq_receive(msg_queue, buffer, MAX_BUFFER_SIZE, 0) > 0)
	 	{
	 		pthread_mutex_lock(&lock);
	 		fprintf(file_ptr,"%s",buffer);
	 		fflush(file_ptr);
	 		pthread_mutex_unlock(&lock);
	 	}
	}
	uart_close(uart1);
		


}


