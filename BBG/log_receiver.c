#include "log_receiver.h"

void *revecive_thread_callback()
{
	printf("Inside receiver thread\n");
	
	uart_properties *uart1  = malloc(sizeof(uart_properties));
	uart1->uart_no = 1;
	uart1->baudrate = B115200;

	uint8_t isOpen1 = uart_config(uart1);
	
	char log[15];
	char b = 's';

	while(1)
	{
		memset(log,'\0', sizeof(log));
		if(uart_receive_task(uart1,&log,sizeof(log)) > 0)
		{

		}
	}
	uart_close(uart1);	
}