/* FreeRTOS 8.2 Tiva Demo
 *
 * main.c
 *
 * Steve Antony
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "driverlib/gpio.h"
#include "driverlib/inc/hw_memmap.h"
#include "log.h"
#include "temp.h"
#include "spi.h"
#include "object_detection.h"
#include "uart.h"
#include "interrupt.h"
#include "rom.h"
#include "driverlib/fpu.h"
#include "motor_driver.h"

/**********************************************
 *        Function Prototypes
 **********************************************/
/********************************************
 * Func name :   LEDTask
 * Parameters:   none
 * Description : Thread for LED task
 ********************************************/
void LEDTask(void *pvParameters);
void ConfigureUART2();
void ConfigureUART1();
/********************************************
 * Func name :   vTimerCallback_LED_handler
 * Parameters:   none
 * Description : timer handler for LED timer
 ********************************************/
void vTimerCallback_LED_handler( TimerHandle_t  *pxTimer );

void Actuator_motor(void *pvParameters);
void ReadUartTask(void *pvParameters);
void Actuator_hot_h2o(void *pvParameters);
void Actuator_night(void *pvParameters);

/**********************************************
 *        Globals
 **********************************************/
QueueHandle_t myQueue_ultra, myQueue_light;

struct log_struct_led log_led;
uint32_t output_clock_rate_hz;

//For temperature notification
TaskHandle_t handle_hot_h2o,handle_motor,handle_night;


/**********************************************
 *        Globals
 **********************************************/
int FLAG_LED = pdFALSE;
QueueHandle_t myQueue_temp, myQueue_led, myQueue_alert;
struct log_struct_led log_led;
uint32_t output_clock_rate_hz;
TaskHandle_t handle;

/**********************************************
 *        Main Function
 **********************************************/





int main(void)
{
    // Initialize system clock to 120 MHz
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);
    FPUEnable();

    // Set up the UART which is connected to the virtual COM port


    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);

    queue_init();


    ConfigureUART1();
    ConfigureUART2();


    // Create light task
   xTaskCreate(LightTask, (const portCHAR *)"Light",
               configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Create logger task
    xTaskCreate(LogTask, (const portCHAR *)"Log",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);


    // Create temp task
    xTaskCreate(UtrasonicTask, (const portCHAR *)"ultrasonic",
                       configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Create temp task
       xTaskCreate(ReadUartTask, (const portCHAR *)"UART",
                          configMINIMAL_STACK_SIZE, NULL, 1, NULL);

       xTaskCreate(Actuator_motor, (const portCHAR *)"motion",
                                 configMINIMAL_STACK_SIZE, NULL, 1, &handle_motor);

       //xTaskCreate(Actuator_night, (const portCHAR *)"auto-on",
             //                           configMINIMAL_STACK_SIZE, NULL, 1, &handle_night);

      // xTaskCreate(Actuator_hot_h2o, (const portCHAR *)"hot_water",
                                     //          configMINIMAL_STACK_SIZE, NULL, 1, &handle_hot_h2o);


    /*start the schedule*/
    vTaskStartScheduler();

    return 0;

}

void ReadUartTask(void *pvParameters)
{
    for(;;)
        {
            while(UARTCharsAvail(UART1_BASE))
            {
                char c = ROM_UARTCharGet(UART1_BASE);
                if(c == '1')
                {
                   // UARTprintf("Object detected %c\n", c);
                    xTaskNotifyGive(handle_motor);
                }
//                if(c == '2')
//                {
//                    UARTprintf("night time %c\n", c);
//                    xTaskNotifyGive(handle_night);
//                }
//                if(c == '3')
//                {
//                    UARTprintf("Water heated %c\n", c);
//                    xTaskNotifyGive(handle_hot_h2o);
//                }

            }
    }

}


void Actuator_night(void *pvParameters)
{
        uint32_t ulNotifiedValue = 0;

        ulNotifiedValue  = ulTaskNotifyTake( pdTRUE, 1000  );
        if(ulNotifiedValue > 0)
        {
            UARTprintf("on the robot\n");
        }

}

void Actuator_motor(void *pvParameters)
{
        init_motor();

        for(;;)
        {
            uint32_t ulNotifiedValue = 0;

            ulNotifiedValue  = ulTaskNotifyTake( pdTRUE, 1  );
            if(ulNotifiedValue > 0)
            {
                UARTprintf("Object detected notified\n");
                //object detected
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);

                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);


               // vTaskDelay(10000/portTICK_PERIOD_MS);
                //normal run of motors
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);

                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
            }





        }
}

void Actuator_hot_h2o(void *pvParameters)
{
    uint32_t ulNotifiedValue = 0;

            ulNotifiedValue  = ulTaskNotifyTake( pdTRUE, 1000  );
            if(ulNotifiedValue > 0)
            {
                UARTprintf("Ready to clean\n");
            }

}
/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}



//Transmit
void ConfigureUART2(void)
{
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //Enable GPIO

        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);    //Enable UART0

        ROM_GPIOPinConfigure(GPIO_PA6_U2RX);                //Configure UART pins
        ROM_GPIOPinConfigure(GPIO_PA7_U2TX);
        ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

        ROM_UARTConfigSetExpClk(UART2_BASE, output_clock_rate_hz, 115200,
                                    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                                UART_CONFIG_PAR_NONE));

        UARTprintf("configured 2\n");

}




//Receive
void ConfigureUART1()
{

        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);    //Enable GPIO

        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);    //Enable UART0

        ROM_GPIOPinConfigure(GPIO_PB0_U1RX);                //Configure UART pins
        ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
        ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);


        ROM_UARTConfigSetExpClk(UART1_BASE, output_clock_rate_hz, 115200,
                                    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                                UART_CONFIG_PAR_NONE));

        UARTprintf("configured 1\n");


//

}

