/**
 * @\file	log_receiver.c
 * @\author	Sanju Prakash Kannioth and Steve Antony X
 * @\brief	This files contains the function definitions for tiva log receiver module
 * @\date	03/30/2019
 *
 */

#include "log_receiver.h"


/**
--------------------------------------------------------------------------------------------
revecive_thread_callback
--------------------------------------------------------------------------------------------
*   This function is the thread callback function for logger messages coming from Tiva
*
*   @\param         void
*
*   @\return        void
*
*/
void *revecive_thread_callback()
{
	char buffer[MAX_BUFFER_SIZE];


	memset(buffer,'\0',sizeof(buffer));
    sprintf(buffer,"DEBUG CN [%s] Tiva receive logger thread active\n", time_stamp());
    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);

	printf("Inside receiver thread\n");
	
	/* Configure UART1 on BBG */
	uart_properties *uart1  = malloc(sizeof(uart_properties));
	uart1->uart_no = 1;
	uart1->baudrate = B115200;

	uint8_t isOpen1 = uart_config(uart1);
	
	char log[50];

	if(isOpen1 == 0)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"DEBUG CN [%s] UART1 Opened successfully\n", time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);

		while(1)
		{
			memset(log,'\0', sizeof(log));

			uart_receive_task(uart1,&log,sizeof(log));
			
		}
	}
	uart_close(uart1); // Close UART1
	return 0;
}
