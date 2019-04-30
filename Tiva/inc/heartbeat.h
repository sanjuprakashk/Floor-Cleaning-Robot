/*
 * heartbeat.h
 *
 *  Created on: Apr 23, 2019
 *      Author: Steve
 */

#ifndef INC_HEARTBEAT_H_
#define INC_HEARTBEAT_H_

/**********************************************
 *               Includes
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

#include "motor_driver.h"

/**********************************************
 *               Function Prototypes
 **********************************************/
/*Heartbeat task*/
void Control_Node_heartbeat(void *pvParameters);

/*Heartbeat Timer handler*/
void vTimerCallback_HB_handler( TimerHandle_t  *pxTimer );

/**********************************************
 *               Global declarations
 **********************************************/
extern QueueHandle_t myQueue_heartbeat;
extern uint32_t DEGRADED_MODE_MANUAL;

#endif /* INC_HEARTBEAT_H_ */

