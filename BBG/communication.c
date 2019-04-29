#include "communication.h"

timer_t  timer_id_heartbeat;

#define HEART_BEAT_CHECK_PERIOD (900000000)//0.9 s

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

int tiva_active, tiva_active_prev;
void beat_timer_handler(union sigval val)
{
	char buffer[MAX_BUFFER_SIZE];
	//restarting the heartbeat timer
	if(tiva_active <= tiva_active_prev)
	{
		printf("TIVA not active\n");
	}

	tiva_active_prev = tiva_active;

	if((kick_timer(timer_id_heartbeat, HEART_BEAT_CHECK_PERIOD)) == -1)
	{
		perror("Error on kicking timer for heartbeat\n");
	}
	
}


void *communication_thread_callback()
{
	tiva_active = 0;

	tiva_active_prev = 0;

	printf("Inside communication thread\n");

	struct sensor_struct comm;

	struct sensor_struct sensor;

	struct logger_struct logger;

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

	if((setup_timer_POSIX(&timer_id_heartbeat,beat_timer_handler)) == -1)
	{
		perror("Error on creating timer for heartbeat\n");
	}

	if((kick_timer(timer_id_heartbeat, HEART_BEAT_CHECK_PERIOD)) == -1)
	{
		perror("Error on kicking timer for heartbeat\n");
	}

	char task[15];
	if (isOpen4 == 0) {
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



				// usleep(10000);
			if(uart_receive(uart4,&sensor, sizeof(sensor_struct)) > 0)
			{

				tiva_active++;
				if((strcmp(sensor.task_name,"DIST") == 0) && sensor.sensor_data < 30)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &obj_detect, sizeof(char));
					pthread_mutex_unlock(&lock_res);
				}

				if((strcmp(sensor.task_name,"LUX") == 0) && sensor.sensor_data < 10)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &lux_auto, sizeof(char));
					pthread_mutex_unlock(&lock_res);
				}

				if((strcmp(sensor.task_name,"WAT") == 0) && sensor.sensor_data < 300 && already_closed == 0)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_close, sizeof(char));
					pthread_mutex_unlock(&lock_res);
					already_closed = 1;
					already_open = 0;
				}
			

				if((strcmp(sensor.task_name,"WAT") == 0) && sensor.sensor_data >= 300 && already_open == 0)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_open, sizeof(char));
					pthread_mutex_unlock(&lock_res);
					already_open = 1;
					already_closed = 0;
				}
			/*	else
				{
				
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_close, sizeof(char));
					pthread_mutex_unlock(&lock_res);
				}
			*/
			}

		}
		uart_close(uart4);
	}
	printf("EOF\n");
	// return 0;
}
