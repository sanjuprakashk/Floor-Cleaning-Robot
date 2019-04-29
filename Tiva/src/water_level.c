/*
 * water_level.c
 *
 *  Created on: Apr 24, 2019
 *      Author: Steve
 */

#include "waterlevel.h"

int FLAG_WL = 0;
static char buffer_log[BUFFER];
char temp_buffer[100];

void Water_level(void *pvParameters)
{
    UARTprintf("Water level task\n");
    uint32_t Water_level_data;

    init_valve();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                                ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 3);

    ADCIntClear(ADC0_BASE, 3);

    long x_WaterL_id = 1009;
    xTimerHandle xTimer_WL;
    xTimer_WL = xTimerCreate("Waterlevel_timer",               // Just a text name, not used by the kernel.
                                pdMS_TO_TICKS( 2000 ),     // 1000ms
                                pdTRUE,                    // The timers will auto-reload themselves when they expire.
                                ( void * ) x_WaterL_id,      // Assign each timer a unique id equal to its array index.
                                vTimerCallback_WaterLevel_handler// Each timer calls the same callback when it expires.
                               );



        if( (xTimer_WL == NULL ) )
        {
            // The timer was not created.
            UARTprintf("Error on timer creation - xTimer_WL\n");
        }

    /*start the timer*/
    xTimerStart( xTimer_WL, 0 );

    while(1)
    {
        if(FLAG_WL)
        {
            ADCProcessorTrigger(ADC0_BASE, 3);


            while(!ADCIntStatus(ADC0_BASE, 3, false))
               {
               }
            ADCIntClear(ADC0_BASE, 3);

            ADCSequenceDataGet(ADC0_BASE, 3, &Water_level_data);


            if(CN_ACTIVE)
             {
                 xQueueSendToBack( myQueue_water,( void * ) &Water_level_data, QUEUE_TIMEOUT_TICKS ) ;
                 memset(buffer_log,'\0',BUFFER);
                 sprintf(buffer_log,"W %d\n",Water_level_data);
                 LOG_INFO(buffer_log)
             }


            FLAG_WL = pdFALSE;

        }
    }
}
void vTimerCallback_WaterLevel_handler( TimerHandle_t  *pxTimer )
{
    FLAG_WL = pdTRUE;
}

void init_valve()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);

    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_7);
    GPIOPadConfigSet(GPIO_PORTK_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}
void open_value()
{
    GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_7, GPIO_PIN_7);
    UARTprintf("CN: Valve opened\n");
    LOG_ERROR("Valve opened")
}

void close_value()
{
    GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_7, 0);
    UARTprintf("CN: Valve closed\n");
    LOG_ERROR("Valve closed")
}
