/*
 * waterlevel.h
 *
 *  Created on: Apr 24, 2019
 *      Author: Steve
 */

#ifndef INC_WATERLEVEL_H_
#define INC_WATERLEVEL_H_


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
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "driverlib/adc.h"
#include "log.h"


/**********************************************
 *        Global declaration
 **********************************************/
extern QueueHandle_t myQueue_water;


/**********************************************
 *        Function prototypes
 **********************************************/
/********************************************
         Water level task
 ********************************************/
void Water_level(void *pvParameters);

/********************************************
      Timer callback for water level task
 ********************************************/
void vTimerCallback_WaterLevel_handler( TimerHandle_t  *pxTimer );

/********************************************
 * Func name :   init_valve
 * Parameters:   none
 * Description : initiates the valve control pin
 ********************************************/
void init_valve();

/********************************************
 * Func name :   close_value
 * Parameters:   none
 * Description : close the water valve
 ********************************************/
void close_value();

/********************************************
 * Func name :   open_value
 * Parameters:   none
 * Description : open the water valve
 ********************************************/
void open_value();


#endif /* INC_WATERLEVEL_H_ */
