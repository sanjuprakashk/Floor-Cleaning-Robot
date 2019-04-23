#include "logger.h"



void *logger_thread_callback()
{
	printf("Inside logger thread");
	uart_properties *uart1  = malloc(sizeof(uart_properties));
	uart1->uart_no = 1;
	uart1->baudrate = B115200;

	uint8_t isOpen1 = uart_config(uart1);

	char a =' ';
	char b = 's';
	
	while(1)
	{
		//uart_send(uart1,&b,sizeof(char));

		usleep(100000);
		if(uart_receive_task(uart1,&a,sizeof(char)) > 0)
		{
		//	printf("UART1\n");
		}
	}
	uart_close(uart1);
		


}


