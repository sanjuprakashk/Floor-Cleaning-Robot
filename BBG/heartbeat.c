#include "heartbeat.h"

void beat_timer_handler(union sigval val)
{
	char buffer[MAX_BUFFER_SIZE];
	//restarting the heartbeat timer
	if(tiva_active <= tiva_active_prev)
	{
		tiva_dead = 1;
		printf("ERROR Tiva dead\n");
		strcpy(buffer,"ERROR Tiva dead\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		tiva_dead = 0;
		strcpy(buffer,"INFO Tiva alive\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		printf("INFO Tiva alive\n");
	}

	if(distance_active <= distance_active_prev)
	{
		distance_dead = 1;
		printf("ERROR Ultrasonic dead\n");
		strcpy(buffer,"ERROR Ultrasonic dead\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		distance_dead = 0;
		strcpy(buffer,"INFO Ultrasonic alive\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		printf("INFO Ultrasonic alive\n");
	}

	if(lux_active <= lux_active_prev)
	{
		lux_dead = 1;
		printf("ERROR Lux dead\n");
		strcpy(buffer,"ERROR Lux dead\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		lux_dead = 0;
		strcpy(buffer,"INFO Lux alive\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		printf("INFO Lux alive\n");
	}

	if(water_active <= water_active_prev)
	{
		water_dead = 1;
		printf("ERROR Water dead\n");
		strcpy(buffer,"ERROR Water dead\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		water_dead = 0;
		strcpy(buffer,"INFO Water alive\n");
		mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		printf("INFO Water alive\n");
	}


	tiva_active_prev = tiva_active;
	distance_active_prev = distance_active;
	lux_active_prev = lux_active;
	water_active_prev = water_active;



	if((kick_timer(timer_id_heartbeat, TIVA_HEART_BEAT_CHECK_PERIOD)) == -1)
	{
		perror("Error on kicking timer for heartbeat\n");
	}
	
}

void *heartbeat_thread_callback()
{
	printf("Inside heartbeat thread\n");

	if((setup_timer_POSIX(&timer_id_heartbeat,beat_timer_handler)) == -1)
	{
		perror("Error on creating timer for heartbeat\n");
	}

	if((kick_timer(timer_id_heartbeat, TIVA_HEART_BEAT_CHECK_PERIOD)) == -1)
	{
		perror("Error on kicking timer for heartbeat\n");
	}

	uart2  = malloc(sizeof(uart_properties));
	uart2->uart_no = 2;
	uart2->baudrate = B115200;

	uint8_t isOpen2 = uart_config(uart2);
	int8_t success;

	printf("IS OPEN 2: %d\n",isOpen2);
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
