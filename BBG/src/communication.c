/**
 * @\file	communication.c
 * @\author	Sanju Prakash Kannioth
 * @\brief	This files contains the definitions for the communication interface for tiva and BBG
 * @\date	04/29/2019
 *
 */
#include "communication.h"

/*
--------------------------------------------------------------------------------------------
get_lux
--------------------------------------------------------------------------------------------
*	This function returns the most updated lux value in string format
*
* 	@\param			void
*
* 	@\return		string containing most recent lux value
*
*/
char *get_lux()
{
	memset(lux,'\0',sizeof(lux));
	sprintf(lux,"%f",comm_rec.lux);
	return lux;
}


/*
--------------------------------------------------------------------------------------------
get_distance
--------------------------------------------------------------------------------------------
*	This function returns the most updated distance value in string format
*
* 	@\param			void
*
* 	@\return		string containing most recent distance value
*
*/
char *get_distance()
{
	memset(distance,'\0',sizeof(distance));
	sprintf(distance,"%f",comm_rec.distance);
	return distance;
} 


/*
--------------------------------------------------------------------------------------------
get_waterLevel
--------------------------------------------------------------------------------------------
*	This function returns the most updated water level value in string format
*
* 	@\param			void
*
* 	@\return		string containing most recent water level value
*
*/
char *get_waterLevel()
{
	memset(waterLevel,'\0', sizeof(waterLevel));
	sprintf(waterLevel,"%f", comm_rec.waterLevel);
	return waterLevel;
}


/*
--------------------------------------------------------------------------------------------
get_mode
--------------------------------------------------------------------------------------------
*	This function returns the most updated operating mode in string format
*
* 	@\param			void
*
* 	@\return		string containing most updated operating mode value
*
*/
char *get_mode()
{
	memset(mode,'\0', sizeof(mode));
	if(!comm_rec.mode)
		sprintf(mode,"Auto");
	else if(comm_rec.mode)
		sprintf(mode,"Manual");
	return mode;
}

/*
--------------------------------------------------------------------------------------------
get_dgMode
--------------------------------------------------------------------------------------------
*	This function returns if the system is in degraded mode in string format
*
* 	@\param			void
*
* 	@\return		string containing degraded mode value
*
*/
char *get_dgMode()
{
	memset(dg_mode,'\0', sizeof(dg_mode));
	if(!comm_rec.dg_mode)
		sprintf(dg_mode,"Normal");
	else if(comm_rec.dg_mode)
		sprintf(dg_mode,"Degraded");

	return dg_mode;
}

/*
--------------------------------------------------------------------------------------------
get_opStatus_tiva
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the remote node
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the remote node
*
*/
char *get_opStatus_tiva()
{
	memset(tiva_opstatus,'\0', sizeof(tiva_opstatus));
	if(!tiva_dead)
		sprintf(tiva_opstatus,"Alive");
	if(tiva_dead)
		sprintf(tiva_opstatus,"Dead");

	return tiva_opstatus;
}


/*
--------------------------------------------------------------------------------------------
get_opStatus_distance
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the distance sensor
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the distance sensor
*
*/
char *get_opStatus_distance()
{
	memset(distance_opstatus,'\0', sizeof(distance_opstatus));
	if(!distance_dead)
		sprintf(distance_opstatus,"Alive");
	if(distance_dead)
		sprintf(distance_opstatus,"Dead");

	return distance_opstatus;
}


/*
--------------------------------------------------------------------------------------------
get_opStatus_lux
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the lux sensor
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the lux sensor
*
*/
char *get_opStatus_lux()
{

	memset(lux_opstatus,'\0', sizeof(lux_opstatus));
	if(!lux_dead)
		sprintf(lux_opstatus,"Alive");
	if(lux_dead)
		sprintf(lux_opstatus,"Dead");

	return lux_opstatus;
}


/*
--------------------------------------------------------------------------------------------
get_opStatus_water
--------------------------------------------------------------------------------------------
*	This function returns the most updated operation status of the water level sensor
*
* 	@\param			void
*
* 	@\return		string containing the most updated operation status of the water level sensor
*
*/
char *get_opStatus_water()
{
	memset(water_opstatus,'\0', sizeof(water_opstatus));
	if(water_dead || water_outOfRange)
		sprintf(water_opstatus,"Dead");
	else
		sprintf(water_opstatus,"Alive");
	

	return water_opstatus;
}


/*
--------------------------------------------------------------------------------------------
get_valveStatus
--------------------------------------------------------------------------------------------
*	This function returns the most updated status of the valve
*
* 	@\param			void
*
* 	@\return		string containing the most updated status of the valve
*
*/
char *get_valveStatus()
{
	memset(water_opstatus,'\0', sizeof(water_opstatus));
	if(already_open)
		sprintf(valve_status,"Open");
	else if(already_closed)
		sprintf(valve_status,"Closed");

	return valve_status;
	
}



/*
--------------------------------------------------------------------------------------------
communication_thread_callback
--------------------------------------------------------------------------------------------
*	This is the thread creation callback function for the communication thread
*
* 	@\param			void
*
* 	@\return		void
*
*/
void *communication_thread_callback()
{
	char buffer[MAX_BUFFER_SIZE];
	/* Flags to check tiva and sensor heartbeats */
	tiva_active = 0;

	tiva_active_prev = 0;

	distance_active_prev = 0;

	distance_active = 0;
	
	lux_active_prev = 0;
	
	lux_active = 0;
	
	water_active_prev = 0;
    
    water_active = 0;

    memset(buffer,'\0',sizeof(buffer));
    sprintf(buffer,"DEBUG CN [%s] Communication thread active\n", time_stamp());
    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
	printf("Inside communication thread\n");

	struct sensor_struct sensor;

	/* Configure UART4 on BBG */
	uart_properties *uart4 = malloc(sizeof(uart_properties));
	uart4->uart_no = 4;
	uart4->baudrate = B115200;

	uint8_t isOpen4 = uart_config(uart4);

	printf("Open success? %d\n", isOpen4);

	/* Messages to be sent to Tiva for closed loop control */
	char obj_detect = '1';
	char valve_close = '2';
	char valve_open = '3';
	char lux_auto = '4';

	int recv = 0;


	if (isOpen4 == 0) {
		memset(buffer,'\0',sizeof(buffer));
	    sprintf(buffer,"DEBUG CN [%s] UART4 Opened successfully\n", time_stamp());
	    mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
		
		while(1)
		{
			recv = uart_receive(uart4,&sensor, sizeof(sensor_struct)); // Receive sensor data from Tiva on BBG UART4
			
			if(recv > 0)
			{

				/* Send object detected message to Tiva if object is detected */
				if((strcmp(sensor.task_name,"DIST") == 0) && comm_rec.distance< 30)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &obj_detect, sizeof(char));
					pthread_mutex_unlock(&lock_res);
					
					memset(buffer,'\0',sizeof(buffer));
					sprintf(buffer,"WARN CN [%s] Object detected sent from CN\n", time_stamp());
					mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
				}

				/* Send lux auto mode on to Tiva if lux is below threshold */
				else if((strcmp(sensor.task_name,"LUX") == 0) && comm_rec.lux < 10)
				{

					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &lux_auto, sizeof(char));
					pthread_mutex_unlock(&lock_res);

					memset(buffer,'\0',sizeof(buffer));
					sprintf(buffer,"DEBUG CN [%s] Lux auto mode sent from CN\n", time_stamp());
					mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
				}

				/* If water level sensor is inactive, or falls out of range */
				else if((strcmp(sensor.task_name,"WAT") == 0) && comm_rec.waterLevel > 3000)
				{
					water_outOfRange = 1;

					memset(buffer,'\0',sizeof(buffer));
					sprintf(buffer,"ERROR CN [%s] Water level out of range\n", time_stamp());
					mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);
				}

				/* Send valve closed to Tiva if water level is below threshold */
				else if((strcmp(sensor.task_name,"WAT") == 0) && comm_rec.waterLevel < 300 && already_closed == 0)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_close, sizeof(char));
					pthread_mutex_unlock(&lock_res);

					memset(buffer,'\0',sizeof(buffer));
					sprintf(buffer,"DEBUG CN [%s]  Valve close sent from CN\n", time_stamp());
					mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);

					already_closed = 1;
					already_open = 0;
					water_outOfRange = 0;
				}
			
				/* Send valve open to Tiva if water level is above threshold */
				else if((strcmp(sensor.task_name,"WAT") == 0) && comm_rec.waterLevel >= 300 && already_open == 0)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_open, sizeof(char));
					pthread_mutex_unlock(&lock_res);

					memset(buffer,'\0',sizeof(buffer));
					sprintf(buffer,"DEBUG CN [%s]  Valve open sent from CN\n", time_stamp());
					mq_send(msg_queue, buffer, MAX_BUFFER_SIZE, 0);

					already_open = 1;
					already_closed = 0;
					water_outOfRange = 0;
				}

			}

		}
		uart_close(uart4); // Close UART4 file descriptor
		pthread_cancel(communication_thread);
	}
	return 0;
}
