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
char log_data_recv[100];
typedef struct
{
    char task[5];
    uint32_t time_stamp;
    float sensor_value;
    int8_t mode_RN;
    int8_t Deg_mode;


}send_sensor_data;


send_sensor_data tx_data;



float lux_recv, distance_recv;
uint32_t Water_level_recv;
int8_t beat_recv;
extern uint32_t DEGRADED_MODE_MANUAL;
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

    myQueue_water = xQueueCreate(QueueLength, sizeof(uint32_t));
    if(myQueue_water == NULL)
    {
        UARTprintf("error on queue creation myQueue_water\n");
    }

    myQueue_log = xQueueCreate(QueueLength, 100);
    if(myQueue_log == NULL)
    {
        UARTprintf("error on queue creation myQueue_ultra\n");
    }

    myQueue_heartbeat = xQueueCreate(QueueLength, sizeof(int8_t));
    if(myQueue_heartbeat == NULL)
    {
        UARTprintf("error on queue creation myQueue_heartbeat\n");
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
            tx_data.mode_RN = mode;
            tx_data.Deg_mode = DEGRADED_MODE_MANUAL;
            UART_send(ptr, sizeof(tx_data));

        }

        if(xQueueReceive(myQueue_ultra, &distance_recv, TIMEOUT_TICKS ) == pdTRUE )
        {
            strcpy(tx_data.task,"DIST");
            tx_data.sensor_value = distance_recv;
            tx_data.time_stamp = xTaskGetTickCount();
            tx_data.mode_RN = mode;
            tx_data.Deg_mode = DEGRADED_MODE_MANUAL;
            UART_send(ptr, sizeof(tx_data));

        }

        if(xQueueReceive(myQueue_water, &Water_level_recv, TIMEOUT_TICKS ) == pdTRUE )
        {
            strcpy(tx_data.task,"WAT");
            tx_data.sensor_value = (float)Water_level_recv;
            tx_data.time_stamp = xTaskGetTickCount();
            tx_data.mode_RN = mode;
            tx_data.Deg_mode = DEGRADED_MODE_MANUAL;

            UART_send(ptr, sizeof(tx_data));

        }

        if(xQueueReceive(myQueue_heartbeat, &beat_recv, TIMEOUT_TICKS ) == pdTRUE )
        {
            UARTprintf("BEA\n");
            strcpy(tx_data.task,"BEA");
            tx_data.mode_RN = mode;
            tx_data.Deg_mode = DEGRADED_MODE_MANUAL;
            tx_data.time_stamp = xTaskGetTickCount();
            tx_data.sensor_value = 0.0;
            UART_send(ptr, sizeof(tx_data));

        }


        memset(log_data_recv,'\0',sizeof(log_data_recv));
        if(xQueueReceive(myQueue_log, log_data_recv, TIMEOUT_TICKS ) == pdTRUE )
        {

//            UARTprintf("--> Log  %s\n",log_data_recv);
            if(CN_ACTIVE == pdTRUE)
            {
                UART_send_log(ptr1, sizeof(log_data_recv)+5);
            }


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


void UART_send_log(char* ptr, int len)
{
    while(len != 0)
    {
        UARTCharPut(UART3_BASE, *ptr);
        ptr++;
        len--;
    }
}

