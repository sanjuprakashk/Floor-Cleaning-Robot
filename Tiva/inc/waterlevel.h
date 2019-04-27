/*
 * waterlevel.h
 *
 *  Created on: Apr 24, 2019
 *      Author: Steve
 */

#ifndef INC_WATERLEVEL_H_
#define INC_WATERLEVEL_H_


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

extern QueueHandle_t myQueue_water;

void Water_level(void *pvParameters);
void vTimerCallback_WaterLevel_handler( TimerHandle_t  *pxTimer );
void init_valve();
void close_value();
void open_value();


#endif /* INC_WATERLEVEL_H_ */
