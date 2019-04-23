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

	struct sensor_struct comm;

	struct sensor_struct sensor;

	struct logger_struct logger;

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
	char task[15];
	if (isOpen1 == 0) {
		unsigned char receive[30];
		while(1)
		{
		//	printf("Entered while\n");
			char buf[30];
			// sprintf(buf, "foo %d", ++i);

		/*	strcpy(sensor.task_name,"LUX");
			sensor.timeStamp = 100;
			sensor.sensor_data = 9999;

			// Send data to uart1
			if (uart_send(uart1, &sensor, sizeof(sensor_struct)) < 0) {
				printf("Could not send data to uart port");
				return -1;
			}

*/
//			usleep(1000);


			memset(&comm,0,sizeof(comm));
			//comm	
			//comm.sensor_data = 0;
			// comm_rec.distance = 0;
		/*	if(uart_receive_task(uart4, &task, sizeof(task)) > 0)
			{
				usleep(10000);
				if(strcmp(task_name,"LUX"))
				{

				}*/
				if(uart_receive(uart4,&sensor, sizeof(sensor_struct)) > 0)
				{
					//printf("\n\n Distance = %f\n\n",comm.sensor);//, comm_rec.distance);
				/*	if((strcmp(logger.task_name,"DIST") == 0) && logger.sensor_data < 10)
					{
						uart_send(uart1, &test, sizeof(char));
					}
					if((strcmp(logger.task_name,"LUX") == 0) && logger.sensor_data < 10)
					{
						uart_send(uart1, &test1, sizeof(char));
					}
					*/
				}

				if(uart_receive(uart1,&logger,sizeof(logger_struct)) > 0)
				{
					printf("UART1\n");
				}
			//}

		/*	strcpy(sensor.task_name,"DIST");
			sensor.timeStamp = 1001;
			sensor.sensor_data = 10.0;

			uart_send(uart1, &sensor, sizeof(sensor_struct));
			usleep(10000);

			uart_receive(uart1, &comm, sizeof(logger_struct));
			
			strcpy(sensor.task_name, "WATER");
			sensor.timeStamp = 1002;
			sensor.sensor_data = 101.1;

			uart_send(uart1, &sensor, sizeof(sensor_struct));

			usleep(10000);
			
			uart_receive(uart1, &comm, sizeof(logger_struct));
			*/
		}
		uart_close(uart1);
		uart_close(uart4);
	}
	printf("EOF\n");
	// return 0;
}
