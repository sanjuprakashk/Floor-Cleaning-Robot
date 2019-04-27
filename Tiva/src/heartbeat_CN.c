

#include "heartbeat.h"

int FLAG_HB = 0;
int CN_ACTIVE = 0;

static uint32_t Pulse = 0, Prev_pulse = 0;

void Control_Node_heartbeat(void *pvParameters)
{
    UARTprintf("Created heartbeat task\n");
    long x_heartbeat_id = 10009;
   xTimerHandle xTimer_HB;
   xTimer_HB = xTimerCreate("Heart_beat",              // Just a text name, not used by the kernel.
                             pdMS_TO_TICKS( 1500 ),     // 100ms
                             pdTRUE,                   // The timers will auto-reload themselves when they expire.
                             ( void * ) x_heartbeat_id,      // Assign each timer a unique id equal to its array index.
                             vTimerCallback_HB_handler// Each timer calls the same callback when it expires.
                            );

    if( xTimer_HB == NULL )
    {
       // The timer was not created.
       UARTprintf("Error on HB timer creation\n");
    }

    xTimerStart( xTimer_HB, 0 );
    for(;;)
    {
        uint32_t ulNotifiedValue = 0;



        ulNotifiedValue  = ulTaskNotifyTake( pdTRUE, 1000  );
        if(ulNotifiedValue > 0)
        {

            Pulse++;
        }

        if(FLAG_HB)
        {
            if(Pulse <= Prev_pulse)
            {
                UARTprintf("Control node dead Pr %d P %d\n",Prev_pulse, Pulse);
                CN_ACTIVE = pdFALSE;
            }
            else
            {
                UARTprintf("Control node active Pr %d P %d\n",Prev_pulse, Pulse);
                CN_ACTIVE = pdTRUE;
            }

            Prev_pulse = Pulse;
            FLAG_HB = pdFALSE;

        }
        if(CN_ACTIVE)
        {
            GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, 0);
            GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, 0);
            GPIOPinWrite(CLP_D3_PORT, CLP_D3_PIN, 0);
            GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, 0);

        }
        else
        {
            GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, CLP_D1_PIN);
            GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, CLP_D2_PIN);
            GPIOPinWrite(CLP_D3_PORT, CLP_D3_PIN, CLP_D3_PIN);
            GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, CLP_D4_PIN);
        }
    }

}


void vTimerCallback_HB_handler( TimerHandle_t  *pxTimer )
{
    FLAG_HB = pdTRUE;
}

