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
#include "object_detection.h"
#include "uart.h"
#include "interrupt.h"
#include "rom.h"
#include "driverlib/fpu.h"
#include "motor_driver.h"
#include "heartbeat.h"
#include "waterlevel.h"
#include "semphr.h"

/**********************************************
 *               Tasks
 **********************************************/

/********************************************
 *         Actuator task
 * Description : Controls the autonomous movement
 *                of the robot in auto mode
 ********************************************/
void Actuator_motor(void *pvParameters);

/********************************************
 *         ReadUart task
 * Description : This tasks reads the control
 *                data which the Control node sends
 ********************************************/
void ReadUartTask(void *pvParameters);

/**********************************************
 *        Globals
 **********************************************/
// for queues that sends data from different tasks to the logger tasks
QueueHandle_t myQueue_ultra, myQueue_light, myQueue_log, myQueue_water, myQueue_heartbeat;

//output clock
uint32_t output_clock_rate_hz;

//For object detection notification and heartbeat notification to get pulses from control node
TaskHandle_t handle_motor,handle_heartbeat;

// flag to start only once based when lux is very low
static uint8_t start_again = 1;

//Flag set when the threads are not created properly
uint8_t STARTUP_FAILED = 0;

/*Sets application mode
 * mode 0  - Auto mode
 * mode 1  - Manual mode
 */
int8_t mode=0; //auto mode on default

//temporary buffer for logger
char temp_buffer[100];

/*mutex to avoid race condition when many tasks use
 * the same queue for logging
 */

SemaphoreHandle_t xSemaphore;

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

    //initiating message queues for communication between various tasks and logger
    queue_init();

    //initiating the semaphore
    xSemaphore = xSemaphoreCreateMutex();

    //configures the uarts UART1, UART2, UART3
    ConfigureUART1();
    ConfigureUART2();
    ConfigureUART3();

    //initiating the motor pins
    init_motor();


    // Create light task
   if(pdPASS != xTaskCreate(LightTask, (const portCHAR *)"Light",
              configMINIMAL_STACK_SIZE, NULL, 1, NULL))
   {
       STARTUP_FAILED = pdTRUE;
       LOG_ERROR("Thread creation failed for LightTask\n")
   }

    // Create logger task
    if(pdPASS != xTaskCreate(LogTask, (const portCHAR *)"Log",
                   configMINIMAL_STACK_SIZE, NULL, 1, NULL))
   {
       STARTUP_FAILED = pdTRUE;
       LOG_ERROR("Thread creation failed for LogTask\n")
   }


    // Create ultrasonic task
    if(pdPASS != xTaskCreate(UtrasonicTask, (const portCHAR *)"ultrasonic",
                       configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        STARTUP_FAILED = pdTRUE;
        DEGRADED_MODE_MANUAL = 1;
        LOG_ERROR("Thread creation failed for UtrasonicTask\n")
    }

    // Create uart task for reading control data from control node
    if(pdPASS != xTaskCreate(ReadUartTask, (const portCHAR *)"UART",
                          configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        STARTUP_FAILED = pdTRUE;
        LOG_ERROR("Thread creation failed for ReadUartTask\n")
    }

    // Create motor actuator task
    if(pdPASS != xTaskCreate(Actuator_motor, (const portCHAR *)"motion",
                                 configMINIMAL_STACK_SIZE, NULL, 1, &handle_motor))
    {
        STARTUP_FAILED = pdTRUE;
        LOG_ERROR("Thread creation failed for Actuator_motor\n")
    }

    // Create heartbeat task
    if(pdPASS != xTaskCreate(Control_Node_heartbeat, (const portCHAR *)"heartbeat",
                                       configMINIMAL_STACK_SIZE, NULL, 1, &handle_heartbeat))
    {
        STARTUP_FAILED = pdTRUE;
        LOG_ERROR("Thread creation failed for Control_Node_heartbeat\n")
    }

    // Create water level task
    if(pdPASS != xTaskCreate(Water_level, (const portCHAR *)"waterlevel",
                                              configMINIMAL_STACK_SIZE, NULL, 1, NULL))
    {
        STARTUP_FAILED = pdTRUE;
        LOG_ERROR("Thread creation failed for Water_level\n")
    }

    //Checks if the threads were created successfully
    if(STARTUP_FAILED == pdTRUE)
    {
        LOG_ERROR("Startup test failed in creating tasks\n")
    }


    /*start the schedule*/
    vTaskStartScheduler();


    return 0;

}

/*Task to receive control data from control node*/
void ReadUartTask(void *pvParameters)
{
    for(;;)
        {
            while(UARTCharsAvail(UART1_BASE))
            {
                char c = ROM_UARTCharGet(UART1_BASE);
                UARTprintf("-> %c\n",c);
                if(c == 'h') //heartbeat
                {
                    xTaskNotifyGive(handle_heartbeat);
                }

                else if((c == '1') && (mode == 0) && (DEGRADED_MODE_MANUAL == 0))//object detected in auto mode
                {
                    xTaskNotifyGive(handle_motor);
                }

                else if(c == '2')//Water level low
                {
                    close_value();
                }

                else if(c == '3')//Water level high
                {
                    open_value();
                }
                else if(c == '4')//auto start - lux
                {
                    if((start_again == 1) && (mode == 0) && (DEGRADED_MODE_MANUAL == 0))
                    {
                        UARTprintf("CN: Auto on of robot\n");
                        LOG_INFO("Auto on of robot\n")
                        forward();
                        start_again = 0;
                    }
                }

                else if(c == 'm') //manual mode
                {
                    UARTprintf("CN: Manual mode\n");
                    LOG_INFO("Switched to Manual mode\n")
                    mode = 1 ;
                    stop();

                }
                else if(c == 'a') //auto mode
                {
                    if((DEGRADED_MODE_MANUAL == 0))
                    {
                        UARTprintf("CN: Auto mode\n");
                        LOG_INFO("Switched to Auto mode\n")
                        mode = 0 ;
                    }

                }

                else if(c == 'u') //forward
                {
                    if(mode == 1)
                    {
                        UARTprintf("CN: forward\n");
                        forward();
                    }

                }
                else if(c == 's') //stop
                {
                   stop();
                   start_again = 1;
                   UARTprintf("CN: stop\n");
                }

                else if(c == 'l') //left
                {
                    if(mode == 1)
                    {
                        left();
                        UARTprintf("CN: left\n");
                        vTaskDelay(300/portTICK_PERIOD_MS);
                        stop();

                    }

                }
                else if(c == 'r') //right
                {
                    if(mode == 1)
                    {
                        right();
                        UARTprintf("CN: right\n");
                        vTaskDelay(300/portTICK_PERIOD_MS);
                        stop();

                    }

                }
                else if(c == 'b') //back
                {
                    if(mode == 1)
                    {
                       backward();
                       UARTprintf("CN: back\n");

                    }

                }
                else if(c == 'o') //force start from phone
                {
                    if((DEGRADED_MODE_MANUAL == 0))
                    {
                        mode = 0 ;
                        forward();
                        UARTprintf("CN: force turn on\n");
                        LOG_INFO("force turn on from phone\n")

                    }

                }
            }
    }

}

/*Actuator task to control motors when an object is detected*/
void Actuator_motor(void *pvParameters)
{

        for(;;)
        {

            uint32_t ulNotifiedValue = 0;

            ulNotifiedValue  = ulTaskNotifyTake( pdTRUE, 0  );
            if(ulNotifiedValue > 0)
            {

                UARTprintf("Object detected notified\n");
                LOG_INFO("Object detected\n")
                backward();


                 vTaskDelay(1000/portTICK_PERIOD_MS);

                //normal run of motors
                right();

                vTaskDelay(500/portTICK_PERIOD_MS);

                forward();

            }

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



/*Uart to transmit sensor data to the control node*/
//Transmit data on PA7
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




/*Uart to receive control data from the control node*/
//UART1 recv on PB0
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




}

//logger send/*Uart to transmit log data to the control node*/
//UART3 tx on PA5
void ConfigureUART3()
{

        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //Enable GPIO

        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);    //Enable UART3

        ROM_GPIOPinConfigure(GPIO_PA5_U3TX);                //Configure UART pins
        ROM_GPIOPinConfigure(GPIO_PA4_U3RX);
        ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4);


        ROM_UARTConfigSetExpClk(UART3_BASE, output_clock_rate_hz, 115200,
                                    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                                UART_CONFIG_PAR_NONE));

        UARTprintf("configured 3\n");




}


