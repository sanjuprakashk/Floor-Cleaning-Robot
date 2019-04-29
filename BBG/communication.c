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

char *get_waterLevel()
{
	memset(waterLevel,' ', sizeof(waterLevel));
	sprintf(waterLevel,"%f", comm_rec.waterLevel);
	return waterLevel;
}

char *get_mode()
{
	memset(mode,' ', sizeof(mode));
	if(!comm_rec.mode)
		sprintf(mode,"Auto");
	if(comm_rec.mode)
		sprintf(mode,"Manual");
	return mode;
}

char *get_dgMode()
{
	memset(dg_mode,' ', sizeof(dg_mode));
	if(!comm_rec.dg_mode)
		sprintf(dg_mode,"Normal");
	else if(comm_rec.dg_mode)
		sprintf(dg_mode,"Degraded");

	return dg_mode;
}

char *get_opStatus_tiva()
{
	memset(tiva_opstatus,' ', sizeof(tiva_opstatus));
	if(!tiva_dead)
		sprintf(tiva_opstatus,"Alive");
	if(tiva_dead)
		sprintf(tiva_opstatus,"Dead");

	return tiva_opstatus;
}

char *get_opStatus_distance()
{
	memset(distance_opstatus,' ', sizeof(distance_opstatus));
	if(!distance_dead)
		sprintf(distance_opstatus,"Alive");
	if(distance_dead)
		sprintf(distance_opstatus,"Dead");

	return distance_opstatus;
}

char *get_opStatus_lux()
{

	memset(lux_opstatus,' ', sizeof(lux_opstatus));
	if(!lux_dead)
		sprintf(lux_opstatus,"Alive");
	if(lux_dead)
		sprintf(lux_opstatus,"Dead");

	return lux_opstatus;
}

char *get_opStatus_water()
{
	memset(water_opstatus,' ', sizeof(water_opstatus));
	if(!water_dead)
		sprintf(water_opstatus,"Alive");
	if(water_dead)
		sprintf(water_opstatus,"Dead");

	return water_opstatus;
}

char *get_valveStatus()
{
	memset(water_opstatus,' ', sizeof(water_opstatus));
	if(already_open)
		sprintf(valve_status,"Open");
	else if(already_closed)
		sprintf(valve_status,"Closed");

	return valve_status;
	
}

void *communication_thread_callback()
{
	char buffer[MAX_BUFFER_SIZE];
	tiva_active = 0;

	tiva_active_prev = 0;

	distance_active_prev = 0;

	distance_active = 0;
	
	lux_active_prev = 0;
	
	lux_active = 0;
	
	water_active_prev = 0;
    
    water_active = 0;

	printf("Inside communication thread\n");

	struct sensor_struct sensor;

	uart_properties *uart4 = malloc(sizeof(uart_properties));
	uart4->uart_no = 4;
	uart4->baudrate = B115200;

	uint8_t isOpen4 = uart_config(uart4);
	int i = 0;
	printf("Open success? %d\n", isOpen4);

	char obj_detect = '1';
	char valve_close = '2';
	char valve_open = '3';
	char lux_auto = '4';

	int recv = 0;


	if (isOpen4 == 0) {
		while(1)
		{
			//	printf("Entered while\n");
		
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



				// usleep(10000);
			//sensor.sensor_data = 0;
			//pthread_mutex_lock(&lock_res);
			recv = uart_receive(uart4,&sensor, sizeof(sensor_struct)); 
			//pthread_mutex_unlock(&lock_res);
			if(recv > 0)
			{

				//tiva_active++;
				if((strcmp(sensor.task_name,"DIST") == 0) && comm_rec.distance< 30)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &obj_detect, sizeof(char));
					pthread_mutex_unlock(&lock_res);
					printf("OBJ DETECTED = %f\n",sensor.distance);
					strcpy(buffer,"WARN Objected detected\n");
					mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
				}

				else if((strcmp(sensor.task_name,"LUX") == 0) && comm_rec.lux < 10)
				{

					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &lux_auto, sizeof(char));
					pthread_mutex_unlock(&lock_res);
					strcpy(buffer,"WARN Lux below threshold\n");
					mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
				}

				else if((strcmp(sensor.task_name,"WAT") == 0) && comm_rec.waterLevel < 300 && already_closed == 0)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_close, sizeof(char));
					pthread_mutex_unlock(&lock_res);
					already_closed = 1;
					already_open = 0;
					printf("ERROR Valve closed = %f\n", comm_rec.waterLevel);
					//strcpy(buffer,"WARN Valve closed\n");
					//mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
				}
			

				else if((strcmp(sensor.task_name,"WAT") == 0) && comm_rec.waterLevel >= 300 && already_open == 0)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_open, sizeof(char));
					pthread_mutex_unlock(&lock_res);
					already_open = 1;
					already_closed = 0;
					printf("ERROR Valve opened = %f\n", comm_rec.waterLevel);
					//strcpy(buffer,"WARN Valve closed\n");
					//mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
				}

				else if(strcmp(sensor.task_name,"BEA") == 0)
				{
					if(sensor.dg_mode == 0)
					{
					//	strcpy(buffer,"INFO In normal operation\n");
					//	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
					}
					else
					{
					//	strcpy(buffer,"WARN Degraded operation\n");
					//	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
					}
				}
			}

		}
		uart_close(uart4);
	}
}
