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
#include <lux.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "portmacro.h"
#include "utils/uartstdio.h"
#include "portmacro.h"
#include "time.h"
#include "semphr.h"
/**********************************************
 *        MACRO
 **********************************************/
#define QueueLength (110)
#define TIMEOUT_TICKS (10)
#define BUFFER (100)


#define LOG_INFO(str) {\
xSemaphoreTake(xSemaphore, 0);\
memset(temp_buffer,'\0',100);\
sprintf(temp_buffer,"INFO RN: [%d] %s",xTaskGetTickCount(),str);\
xQueueSendToBack( myQueue_log,( void * ) temp_buffer, QUEUE_TIMEOUT_TICKS ) ;\
xSemaphoreGive(xSemaphore);\
}

#define LOG_ERROR(str) {\
xSemaphoreTake(xSemaphore, 0);\
memset(temp_buffer,'\0',100);\
sprintf(temp_buffer,"ERROR RN: [%d] %s",xTaskGetTickCount(),str);\
xQueueSendToBack( myQueue_log,( void * ) temp_buffer, QUEUE_TIMEOUT_TICKS ) ;\
xSemaphoreGive(xSemaphore);\
}

#define LOG_WARN(str) {\
xSemaphoreTake(xSemaphore, 0);\
memset(temp_buffer,'\0',100);\
sprintf(temp_buffer,"WARN RN: [%d] %s",xTaskGetTickCount(),str);\
xQueueSendToBack( myQueue_log,( void * ) temp_buffer, QUEUE_TIMEOUT_TICKS ) ;\
xSemaphoreGive(xSemaphore);\
}
/**********************************************
 *        Global declarations
 **********************************************/
extern QueueHandle_t myQueue_light, myQueue_ultra, myQueue_log, myQueue_water, myQueue_heartbeat;
extern int CN_ACTIVE ;
extern int8_t mode;
extern uint32_t DEGRADED_MODE_MANUAL;
extern SemaphoreHandle_t xSemaphore;

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


/********************************************
 * Func name :   UART_send
 * Parameters:   Address, length
 * Description : Uart function to send sensor values to the control node
 ********************************************/
void UART_send(char* ptr, int len);

/********************************************
 * Func name :   UART_send_log
 * Parameters:   Address, length
 * Description : Uart function to send log data to the control node
 ********************************************/
void UART_send_log(char* ptr, int len);
#endif /* LOG_H_ */
