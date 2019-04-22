#include "communication.h"


char *get_lux()
{
	memset(lux,' ',sizeof(lux));
	sprintf(lux,"%f",comm_rec.lux);
	return lux;
}

char *get_distance()
{
	memset(distance,' ',sizeof(distance));
	sprintf(distance,"%f",comm_rec.distance);
	return distance;
}

void *communication_thread_callback()
{

	printf("Inside communication thread\n");

	communication comm;

	uart_properties *uart1  = malloc(sizeof(uart_properties));
	uart1->uart_no = 1;
	uart1->baudrate = B115200;

	uart_properties *uart4 = malloc(sizeof(uart_properties));
	uart4->uart_no = 4;
	uart4->baudrate = B115200;

	uint8_t isOpen1 = uart_config(uart1);
	uint8_t isOpen4 = uart_config(uart4);
	int i = 0;
	printf("Open success? %d\n", isOpen1);

	char test = '1';
	char test1 = '2';
	if (isOpen1 == 0) {
		unsigned char receive[30];
		while(1)
		{
		//	printf("Entered while\n");
			char buf[30];
			// sprintf(buf, "foo %d", ++i);

/*			comm.lux = 100;
			comm.distance = 23;

			// Send data to uart1
			if (uart_send(uart, &comm, sizeof(communication)) < 0) {
				printf("Could not send data to uart port");
				return -1;
			}

*/
			usleep(1000000);


			comm_rec.lux = 0;
			comm_rec.distance = 0;
			if(uart_receive(uart4,&comm_rec, sizeof(communication)) > 0)
			{
				printf("\n\nLux = %f \t\t Distance = %f\n\n",comm_rec.lux,comm_rec.distance);
				if(comm_rec.distance < 10)
				{
					uart_send(uart1, &test, sizeof(char));
				}
				if(comm_rec.lux < 10)
				{
					uart_send(uart1, &test1, sizeof(char));
				}
			}

		}
		uart_close(uart1);
		uart_close(uart4);
	}
	printf("EOF\n");
	return 0;
}
