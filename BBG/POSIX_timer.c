/**
 * @\file	POSIX_timer.c
 * @\author	Steve Antony X
 * @\brief	This files contains the function definitions for POSIX timer 
 * @\date	03/30/2019
 *
 */

/*****************************************************************
						Includes
*****************************************************************/
#include "POSIX_timer.h"

 

/*****************************************************************
					POSIX Timer configuration
*****************************************************************/
int setup_timer_POSIX(timer_t *timer_id,void (*handler)(union sigval))
{
	struct 	sigevent sev;
	sev.sigev_notify = SIGEV_THREAD; //Upon timer expiration, invoke sigev_notify_function
	sev.sigev_notify_function = handler; //this function will be called when timer expires
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = &timer_id;


	if(timer_create(CLOCK_REALTIME, &sev, timer_id) != 0) //on success, timer id is placed in timer_id
	{
		return ERROR;
	}  




    return SUCCESS;
}

/*****************************************************************
					Start configuration
			Parameter : delay in nano secs
*****************************************************************/
int kick_timer(timer_t timer_id, int interval_ns)
{
   struct itimerspec in;

    in.it_value.tv_sec = 2;
    in.it_value.tv_nsec = 0;//interval_ns; //sets initial time period
    in.it_interval.tv_sec = 2;
    in.it_interval.tv_nsec =0;// interval_ns; //sets interval
    
    //issue the periodic timer request here.
    if( (timer_settime(timer_id, 0, &in, NULL)) != SUCCESS)
    {
    	return ERROR;
    }
    return SUCCESS;
}

/*****************************************************************
					Destroy Timer
*****************************************************************/
int stop_timer(timer_t timer_id)
{
	if( (timer_delete(timer_id)) != SUCCESS)
    {
    	printf("Error on delete timer function\n");
    	return ERROR;
    }
	

    return SUCCESS;
}
