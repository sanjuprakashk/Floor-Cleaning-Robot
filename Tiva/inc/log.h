/*
 * log.h
 *
 *  Created on: Apr 8, 2019
 *      Author: Steve Antony
 */

#ifndef LOG_H_
#define LOG_H_

/**********************************************
 *        Includes
 **********************************************/
#include "FreeRTOS.h"
#include "queue.h"
#include "portmacro.h"
#include "utils/uartstdio.h"
#include "portmacro.h"
#include "time.h"
#include "temp.h"
/**********************************************
 *        MACRO
 **********************************************/
#define QueueLength (100)
#define TIMEOUT_TICKS (10)
#define BUFFER (50)

/**********************************************
 *        Global declarations
 **********************************************/
extern QueueHandle_t myQueue_light, myQueue_ultra, myQueue_log;

/**********************************************
 *        Function Prototypes
 **********************************************/
/********************************************
 * Func name : queue_init
 * Parameters: none
 * Description : initiates the queues for logger
 */
void queue_init();

/********************************************
 * Func name :   LogTask
 * Parameters:   none
 * Description : Thread for logger task
 ********************************************/
void LogTask(void *pvParameters);

void UART_send(char* ptr, int len);
void UART_send1(char* ptr, int len);
#endif /* LOG_H_ */
