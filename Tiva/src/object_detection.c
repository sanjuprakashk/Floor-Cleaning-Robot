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

uint32_t start, end;
uint32_t FLAG_UL, sensor_busy;
float time;

void init_ultrasonic_sensor()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    vTaskDelay(pdMS_TO_TICKS( 1 ));
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
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, GPIO_PIN_1);
    vTaskDelay(pdMS_TO_TICKS( 10 ));
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);
}


void PortFIntHandler()
{
    UARTprintf("Entered handler\n");


    taskENTER_CRITICAL();
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_3);




        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_INT_PIN_3) == GPIO_INT_PIN_3)
        {
            HWREG(TIMER2_BASE + TIMER_O_TAV) = 0;
            TimerEnable(TIMER2_BASE,TIMER_A);
            start = 0;
            UARTprintf("start %d\n", (start));

        }
        else
        {
            end = TimerValueGet(TIMER2_BASE,TIMER_A);
            TimerDisable(TIMER2_BASE,TIMER_A);
            //time = end - start;
            UARTprintf("end %d\n", (end));
           // char buf[30];
           // sprintf(buf,"time %f\n", (time));
           // UARTprintf("time %s\n", (buf));


        }




    taskEXIT_CRITICAL();

}

void UtrasonicTask(void *pvParameters)
{
    UARTprintf("Created ultrasonic thread\n");
    long x_ultra_id = 1003;
    xTimerHandle xTimer_ult;
    xTimer_ult = xTimerCreate("Timer_ultrasonic",               // Just a text name, not used by the kernel.
                                pdMS_TO_TICKS( DETECT_TIME_PERIOD_MS ),     // 1000ms
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


         //setup i2c
         init_ultrasonic_sensor();

         for(;;)
         {
             if(FLAG_UL == pdTRUE)
             {
                 find_object();
                 FLAG_UL = pdFALSE;
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
