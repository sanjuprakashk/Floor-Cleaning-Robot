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
struct communication
{
    float lux;
    float distance;
};

struct communication tx;

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



}

/**********************************************
 *        Logger thread
 **********************************************/
void LogTask(void *pvParameters)
{

    char buffer[50];
    unsigned char *ptr;
    ptr = (uint8_t *) (&tx);

    for(;;)
    {
        if(xQueueReceive(myQueue_light, &lux_recv, TIMEOUT_TICKS ) == pdTRUE )
        {

            tx.lux = lux_recv;
            sprintf(buffer, "%f",tx.lux);
            //UARTprintf("Lux %s\n",buffer);



//                UARTprintf("size %d\n",sizeof(tx));

                //UART_send(ptr, sizeof(tx));


        }

        if(xQueueReceive(myQueue_ultra, &distance_recv, TIMEOUT_TICKS ) == pdTRUE )
        {
            tx.distance = distance_recv;
            sprintf(buffer, "%f",tx.distance);
           // UARTprintf("Dis %s\n",buffer);
            UART_send(ptr, sizeof(tx));
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
