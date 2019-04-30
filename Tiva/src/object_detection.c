/*
 * object_detection.c
 *
 *  Created on: Apr 15, 2019
 *      Author: Steve Antony
 */

/**********************************************
 *        Includes
 **********************************************/

#include "object_detection.h"


/**********************************************
 *        GLOBALS
 **********************************************/
//to find the pulse duration
uint32_t start, end;

//conversion complete flag
uint32_t FLAG_UL, conv_complete = 0;

//find the duration of echo on pulse
float time_pulse = 0;

//get the distance
float distance_send;

//for local logger
static char buffer_log[BUFFER];

//flag to indicate if the sensor is dead
uint32_t ULT_DEAD = 0;

//indicate degraded mode
uint32_t DEGRADED_MODE_MANUAL = 0;

//mutex for log
SemaphoreHandle_t xSemaphore;

//local logger buffer
char temp_buffer[100];


void init_ultrasonic_sensor()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);


    //echo pin
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_3);
    //GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_3);

    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_BOTH_EDGES );

    GPIOIntRegister(GPIO_PORTF_BASE,PortFIntHandler);

    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_3);



    //trigger pin
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    UARTprintf("configured ultrasonic\n");

}


void find_object()
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);
    vTaskDelay(pdMS_TO_TICKS( 1 ));
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, GPIO_PIN_1);
    vTaskDelay(pdMS_TO_TICKS( 10 ));
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);
}


void PortFIntHandler()
{


    taskENTER_CRITICAL();
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_3);


        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_INT_PIN_3) == GPIO_INT_PIN_3)
        {
            HWREG(TIMER2_BASE + TIMER_O_TAV) = 0;
            TimerEnable(TIMER2_BASE,TIMER_A);
            start = TimerValueGet(TIMER2_BASE,TIMER_A);
        }

        else
        {

            end = TimerValueGet(TIMER2_BASE,TIMER_A);
            TimerDisable(TIMER2_BASE,TIMER_A);
            time_pulse = end - start;
            conv_complete = 1;


        }

    taskEXIT_CRITICAL();

}

void UtrasonicTask(void *pvParameters)
{
    UARTprintf("Created ultrasonic thread\n");
    long x_ultra_id = 1003;
    xTimerHandle xTimer_ult;
    xTimer_ult = xTimerCreate("Timer_ultrasonic",               // Just a text name, not used by the kernel.
                                pdMS_TO_TICKS( PERIOD_ULTRASONIC ),
                                pdTRUE,                    // The timers will auto-reload themselves when they expire.
                                ( void * ) x_ultra_id,      // Assign each timer a unique id equal to its array index.
                                vTimerCallback_Ultra_handler// Each timer calls the same callback when it expires.
                               );



    if( (xTimer_ult == NULL ) )
    {
        // The timer was not created.
        UARTprintf("Error on timer creation - xTimer_Temp\n");
    }

    else
    {

        /*start the timer*/
         xTimerStart( xTimer_ult, 0 );

         init_ultrasonic_sensor();

         //startup test
         find_object();
         vTaskDelay(500/portTICK_PERIOD_MS);
         if(time_pulse == 0)
         {
             UARTprintf("Startup test failed for ultrasonic sensor\n");
             LOG_ERROR("Killed ultrasonic sensor task - Startup failed\n")
             DEGRADED_MODE_MANUAL = 1;
             mode = 1;
             vTaskDelete( NULL );
         }



         for(;;)
         {
             if(FLAG_UL == pdTRUE)
             {
                 find_object();

                 if((conv_complete == 1))
                 {
                     distance_send = (((float)(1.0/(output_clock_rate_hz/1000000))*time_pulse)/58);

                     if(CN_ACTIVE)
                     {
                         xQueueSendToBack( myQueue_ultra,( void * ) &distance_send, QUEUE_TIMEOUT_TICKS ) ;
                         memset(buffer_log,'\0',BUFFER);
                         sprintf(buffer_log,"D %f\n",distance_send);
                         LOG_INFO(buffer_log)
                     }
                     else
                     {
                         if(distance_send < 30)
                         {
                             //when object detected
                             xTaskNotifyGive(handle_motor);


                         }

                     }
                     conv_complete = 0;

                 }
                 else
                 {
                     ULT_DEAD++;
                 }

                 FLAG_UL = pdFALSE;

             }
             if(ULT_DEAD > 5) //switch to degraded mode
             {
                DEGRADED_MODE_MANUAL = 1;
                mode = 1;
                stop();
                UARTprintf("Killed Utrasonic sensor task\n");
                LOG_ERROR("Killed Utrasonic sensor task\n")
                vTaskDelete( NULL );
             }


         }

    }
}

/**********************************************
 *         Temp timer handler
 **********************************************/
void vTimerCallback_Ultra_handler( TimerHandle_t  *pxTimer )
{
    FLAG_UL = pdTRUE;
}

