/**********************************************
 *               Includes
 **********************************************/
#include "heartbeat.h"

/**********************************************
 *               Globals
 **********************************************/
//timer flag to check the heartbeat after regular intervals
int FLAG_HB = 0;

//flag is set if the control node is active
int CN_ACTIVE = 0;

//for sending heartbeat from Remote node to control node
int8_t BEAT = 1;

//storing pulses to find heartbeat
static uint32_t Pulse = 0, Prev_pulse = 0;

//temporary buffer for logger
char temp_buffer[100];

/*
--------------------------------------------------------------------------------------------
               Control_Node_heartbeat task
--------------------------------------------------------------------------------------------
*   This Task sends heartbeat continuosly from remote node to control node and
*   Checks if the control node is active
*
*/
void Control_Node_heartbeat(void *pvParameters)
{
    UARTprintf("Created heartbeat task\n");
    long x_heartbeat_id = 1019;
   xTimerHandle xTimer_HB;
   xTimer_HB = xTimerCreate("Heart_beat",              // Just a text name, not used by the kernel.
                             pdMS_TO_TICKS( 1000 ),     // 100ms
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


        //notified when heartbeat is received from control node
        ulNotifiedValue  = ulTaskNotifyTake( pdTRUE, 0  );
        if(ulNotifiedValue > 0)
        {

            Pulse++;
        }

        if(FLAG_HB)
        {
            FLAG_HB = pdFALSE;

            //checks if there was a pulse received
            if(Pulse <= Prev_pulse)
            {
               // UARTprintf("Control node dead Pr %d P %d\n",Prev_pulse, Pulse);
                CN_ACTIVE = pdFALSE;
            }
            else
            {
               // UARTprintf("Control node active Pr %d P %d\n",Prev_pulse, Pulse);
                CN_ACTIVE = pdTRUE;
            }

            Prev_pulse = Pulse;

            //send pulse from remote node to control node
            xQueueSendToBack( myQueue_heartbeat,( void * ) &BEAT, QUEUE_TIMEOUT_TICKS ) ;

            //turn off the remote node leds when the control node is active
            if(CN_ACTIVE)
            {
                GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, 0);
                GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, 0);
                GPIOPinWrite(CLP_D3_PORT, CLP_D3_PIN, 0);
                GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, 0);

            }
            //turn on the remote node leds when the control node is active
            else
            {
                GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, CLP_D1_PIN);
                GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, CLP_D2_PIN);
                GPIOPinWrite(CLP_D3_PORT, CLP_D3_PIN, CLP_D3_PIN);
                GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, CLP_D4_PIN);

                //move to fail safe mode from degraded mode when the ultrasonic sensor is dead and control node inactive
                if((DEGRADED_MODE_MANUAL == 1))
                {
                    stop();
                    UARTprintf("System shutdown as no ultrasonic sensor and no control node - Fail safe\n");
                }
            }


        }

    }

}

/*Heartbeat Timer handler*/
void vTimerCallback_HB_handler( TimerHandle_t  *pxTimer )
{
    FLAG_HB = pdTRUE;
}

