/**
 * @\file	heartbeat.c
 * @\author	Sanju Prakash Kannioth
 * @\brief	This files contains the definitions for the heartbeat functions
 * @\date	04/29/2019
 *
 */

#include "heartbeat.h"


/*
--------------------------------------------------------------------------------------------
beat_timer_handler
--------------------------------------------------------------------------------------------
*	This function is the timer handler for tiva heart beat timer
*
* 	@\param			signal value ( dummy)
*
* 	@\return		none
*
*/
void beat_timer_handler(union sigval val)
{
	char buffer[MAX_BUFFER_SIZE];

	if(tiva_active <= tiva_active_prev)
	{
		tiva_dead = 1;
		printf("ERROR Tiva dead\n");

		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"ERROR CN [%s] Tiva Dead\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		tiva_dead = 0;
		printf("INFO Tiva alive\n");

		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"DEBUG CN [%s] Tiva Alive\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}

	if(distance_active <= distance_active_prev)
	{
		distance_dead = 1;
		printf("ERROR Ultrasonic dead\n");

		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"ERROR CN [%s] Ultrasonic Dead\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		distance_dead = 0;

		printf("INFO Ultrasonic alive\n");

		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"DEBUG CN [%s] Ultrasonic Alive\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}

	if(lux_active <= lux_active_prev)
	{
		lux_dead = 1;
		printf("ERROR Lux dead\n");
	
		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"ERROR CN [%s] Lux Dead\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		lux_dead = 0;
		
		printf("INFO Lux alive\n");

		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"DEBUG CN [%s] Lux Alive\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}

	if((water_active <= water_active_prev) || water_outOfRange)
	{
		water_dead = 1;
		printf("ERROR Water dead\n");
		
		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"ERROR CN [%s] Water level Dead\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}
	else
	{
		water_dead = 0;

		printf("INFO Water alive\n");

		memset(buffer,'\0',sizeof(buffer));
    	sprintf(buffer,"DEBUG CN [%s] Water level Alive\n", time_stamp());
    	mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	}


	tiva_active_prev = tiva_active;
	distance_active_prev = distance_active;
	lux_active_prev = lux_active;
	water_active_prev = water_active;



	//restarting the heartbeat timer
	if((kick_timer(timer_id_heartbeat, TIVA_HEART_BEAT_CHECK_PERIOD)) == -1)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Kicking timer for heartbeat failed\n", time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Error on kicking timer for heartbeat\n");
	}
	
}

/*
--------------------------------------------------------------------------------------------
heartbeat_thread_callback
--------------------------------------------------------------------------------------------
*	This is the thread creation callback function for the heartbeat thread
*
* 	@\param			void
*
* 	@\return		void
*
*/
void *heartbeat_thread_callback()
{
	char buffer[MAX_BUFFER_SIZE];

	memset(buffer,'\0',sizeof(buffer));
    sprintf(buffer,"DEBUG CN [%s] Heartbeat thread active\n", time_stamp());
    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	printf("Inside heartbeat thread\n");


	if((setup_timer_POSIX(&timer_id_heartbeat,beat_timer_handler)) == -1)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Creating timer for heartbeat failed\n", time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Error on creating timer for heartbeat\n");
	}

	if((kick_timer(timer_id_heartbeat, TIVA_HEART_BEAT_CHECK_PERIOD)) == -1)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"ERROR CN [%s] Kicking timer for heartbeat failed\n", time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		perror("Error on kicking timer for heartbeat\n");
	}

	/* Configure UART2 on BBG */
	uart2  = malloc(sizeof(uart_properties));
	uart2->uart_no = 2;
	uart2->baudrate = B115200;

	uint8_t isOpen2 = uart_config(uart2);
	
	printf("IS OPEN 2: %d\n",isOpen2);
	char hb ='h';

	if(isOpen2 == 0)
	{
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"DEBUG CN [%s] UART2 Opened successfully\n", time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);

		while(1)
		{
			usleep(1000000); // Sending heartbeat every 1 second

			pthread_mutex_lock(&lock_res);
			uart_send(uart2,&hb,sizeof(char));
			pthread_mutex_unlock(&lock_res);

		}
	}
	uart_close(uart2); // Close UART2
	pthread_cancel(heartbeat_thread); 
	return 0;	

}
