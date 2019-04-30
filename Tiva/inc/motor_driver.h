

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_


/*
 * motor_driver.h
 *
 *  Created on: Apr 14, 2019
 *      Author: Steve Antony
 */

/**********************************************
 *        Includes
 **********************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "timer.h"

#include "driverlib/gpio.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/inc/hw_timer.h"
#include "driverlib/inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "driverlib/inc/hw_ints.h"
#include "driverlib/fpu.h"


/**********************************************
 *        Function Prototypes
 **********************************************/
/*
--------------------------------------------------------------------------------------------
init_motor
--------------------------------------------------------------------------------------------
*   This functions is used to initiate the motor output pins
*
*   @\param         void
*
*   @\return        void
*
*/
void init_motor();

/*
--------------------------------------------------------------------------------------------
stop
--------------------------------------------------------------------------------------------
*   This functions stops the motion of robot
*
*   @\param         void
*
*   @\return        void
*
*/
void stop();

/*
--------------------------------------------------------------------------------------------
forward
--------------------------------------------------------------------------------------------
*   This functions moves the robot forward
*
*   @\param         void
*
*   @\return        void
*
*/
void forward();

/*
--------------------------------------------------------------------------------------------
left
--------------------------------------------------------------------------------------------
*   This functions turns the robot left
*
*   @\param         void
*
*   @\return        void
*
*/
void left();

/*
--------------------------------------------------------------------------------------------
right
--------------------------------------------------------------------------------------------
*   This functions turns the robot right
*
*   @\param         void
*
*   @\return        void
*
*/
void right();

/*
--------------------------------------------------------------------------------------------
backward
--------------------------------------------------------------------------------------------
*   This functions moves the robot backward
*
*   @\param         void
*
*   @\return        void
*
*/
void backward();




#endif
