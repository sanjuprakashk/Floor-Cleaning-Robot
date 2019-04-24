#include "heartbeat.h"

void *heartbeat_thread_callback()
{
	printf("Inside logger thread");
	uart_properties *uart2  = malloc(sizeof(uart_properties));
	uart2->uart_no = 2;
	uart2->baudrate = B115200;

	uint8_t isOpen2 = uart_config(uart2);
	int8_t success;
	char hb ='h';
	while(1)
	{
		usleep(1000000);

		pthread_mutex_lock(&lock_res);
		success = uart_send(uart2,&hb,sizeof(char));
		pthread_mutex_unlock(&lock_res);

		if(success > 0)
		{
			printf("HEARTBEAT SENT\n");
		}
	}
	uart_close(uart2);
		


}

