/*
 * main.h
 *
 *  Created on: Mar 28, 2015
 *      Author: steve
 */

#ifndef OBJECT_DETECTION_H_
#define OBJECT_DETECTION_H_

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
void init_ultrasonic_sensor();
void PortFIntHandler();
void find_object();
void vTimerCallback_handler( TimerHandle_t  *pxTimer );

#define DETECT_TIME_PERIOD_MS (4000)
extern uint32_t output_clock_rate_hz;

/********************************************
 * Func name :   vTimerCallback_Temp_handler
 * Parameters:   none
 * Description : handler for temperature timer
 ********************************************/
void vTimerCallback_Ultra_handler( TimerHandle_t  *);
void UtrasonicTask(void *);

#endif