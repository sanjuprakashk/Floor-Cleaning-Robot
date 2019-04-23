/*
 * log.c
 *
 *  Created on: Apr 8, 2019
 *      Author: Steve Antony
 */
#include "log.h"


/**********************************************
 *        Global definitions
 **********************************************/
char log_data_recv[30];
typedef struct
{
    char task[5];
    uint32_t time_stamp;
    float sensor_value;


}send_sensor_data;

send_sensor_data tx_data;

//typedef struct
//{
//    char task[5];
//    uint32_t time_stamp;
//    char log_data[100];
//
//}send_log_data;
//
//send_log_data tx_log;

//struct communication
//{
//    float lux;
//    float distance;
//};
//
//struct communication tx;

float lux_recv, distance_recv;

void queue_init()
{
    myQueue_light = xQueueCreate(QueueLength, sizeof(float));
    if(myQueue_light == NULL)
    {
        UARTprintf("error on queue creation myQueue_light\n");
    }

    myQueue_ultra = xQueueCreate(QueueLength, sizeof(float));
    if(myQueue_ultra == NULL)
    {
        UARTprintf("error on queue creation myQueue_ultra\n");
    }

    myQueue_log = xQueueCreate(QueueLength, 30);
    if(myQueue_log == NULL)
    {
        UARTprintf("error on queue creation myQueue_ultra\n");
    }
}

/**********************************************
 *        Logger thread
 **********************************************/
void LogTask(void *pvParameters)
{

    char buffer[50];
    unsigned char *ptr;
    ptr = (uint8_t *) (&tx_data);

    unsigned char *ptr1;
        ptr1 = (uint8_t *) (log_data_recv);


    for(;;)
    {
        if(xQueueReceive(myQueue_light, &lux_recv, TIMEOUT_TICKS ) == pdTRUE )
        {
            strcpy(tx_data.task,"LUX");
            tx_data.sensor_value = lux_recv;
            tx_data.time_stamp = xTaskGetTickCount();
            sprintf(buffer, "%f",tx_data.sensor_value);
            UARTprintf("Lux %s\n",buffer);

            UART_send(ptr, sizeof(tx_data));

        }

        if(xQueueReceive(myQueue_ultra, &distance_recv, TIMEOUT_TICKS ) == pdTRUE )
        {
            strcpy(tx_data.task,"DIST");
            tx_data.sensor_value = distance_recv;
            tx_data.time_stamp = xTaskGetTickCount();
            sprintf(buffer, "%f",tx_data.sensor_value);
            UARTprintf("Dis %s\n",buffer);
            UART_send(ptr, sizeof(tx_data));

        }

        memset(log_data_recv,'\0',sizeof(log_data_recv));
        if(xQueueReceive(myQueue_log, log_data_recv, TIMEOUT_TICKS ) == pdTRUE )
        {
//            memset(tx_log.task,'\0',sizeof(tx_log.task));
//            strcpy(tx_log.task,"LOG");
//            memset(tx_log.log_data,'\0',sizeof(tx_log.log_data));
//            strcpy(tx_log.log_data,log_data_recv);

//            tx_log.time_stamp = xTaskGetTickCount();

            UARTprintf("-- Log  %s\n",log_data_recv);
            UART_send1(ptr1, strlen(log_data_recv));

        }
    }
}


void UART_send(char* ptr, int len)
{
    while(len != 0)
    {
        UARTCharPut(UART2_BASE, *ptr);
        ptr++;
        len--;
    }
}


void UART_send1(char* ptr, int len)
{
    while(len != 0)
    {
        UARTCharPut(UART1_BASE, *ptr);
        ptr++;
        len--;
    }
}
