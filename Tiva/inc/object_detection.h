/*
 * main.h
 *
 *  Created on: Mar 28, 2015
 *      Author: steve
 */

#ifndef OBJECT_DETECTION_H_
#define OBJECT_DETECTION_H_

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
#include "log.h"
#include "motor_driver.h"
#include "semphr.h"


/**********************************************
 *        Macros
 **********************************************/
#define DETECT_TIME_PERIOD_MS (1000)
#define PERIOD_ULTRASONIC     (530)


/**********************************************
 *        Function prototypes
 **********************************************/
/********************************************
 * Func name :   init_ultrasonic_sensor
 * Parameters:   none
 * Description : Initiates the trigger and echo pins of ultrasonic sensors
 ********************************************/
void init_ultrasonic_sensor();

/********************************************
 * Func name :   PortFIntHandler
 * Parameters:   none
 * Description : Interrupt handler
 ********************************************/
void PortFIntHandler();

/********************************************
 * Func name :   findobject
 * Parameters:   none
 * Description : makes trigger pin high for 10ms
 ********************************************/
void find_object();

/********************************************
 * Func name :   vTimerCallback_Temp_handler
 * Parameters:   none
 * Description : handler for temperature timer
 ********************************************/
void vTimerCallback_Ultra_handler( TimerHandle_t  *);

/*Ultrasonic task*/
void UtrasonicTask(void *);

/**********************************************
 *        Global declaration
 **********************************************/
extern uint32_t output_clock_rate_hz;
extern QueueHandle_t myQueue_ultra, myQueue_light, myQueue_log;
extern TaskHandle_t handle_motor;



#endif
