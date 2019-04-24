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
#define QueueLength (110)
#define TIMEOUT_TICKS (10)
#define BUFFER (100)
char buffer_log[BUFFER];

#define LOG_INFO(str) {\
char temp_buffer[100];\
memset(temp_buffer,'\0',100);\
sprintf(temp_buffer,"INFO [%d] %s",xTaskGetTickCount(),str);\
xQueueSendToBack( myQueue_log,( void * ) temp_buffer, QUEUE_TIMEOUT_TICKS ) ;\
}

#define LOG_ERROR(str) {\
char temp_buffer[100];\
memset(temp_buffer,'\0',100);\
sprintf(temp_buffer,"ERROR [%d] %s",xTaskGetTickCount(),str);\
xQueueSendToBack( myQueue_log,( void * ) temp_buffer, QUEUE_TIMEOUT_TICKS ) ;\
}

#define LOG_WARN(str) {\
char temp_buffer[30];\
sprintf(temp_buffer,"WARN [%d] %s",xTaskGetTickCount(),str);\
xQueueSendToBack( myQueue_log,( void * ) temp_buffer, QUEUE_TIMEOUT_TICKS ) ;\
}
/**********************************************
 *        Global declarations
 **********************************************/
extern QueueHandle_t myQueue_light, myQueue_ultra, myQueue_log;
extern int CN_ACTIVE ;

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
void UART_send_log(char* ptr, int len);
#endif /* LOG_H_ */
