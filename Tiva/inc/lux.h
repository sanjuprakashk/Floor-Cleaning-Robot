/*
 * temp.h
 *
 *  Created on: Apr 9, 2019
 *      Author: Steve Antony
 */

#ifndef TEMP_H_
#define TEMP_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "utils/uartstdio.h"
#include "uart.h"
#include "driverlib/gpio.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "log.h"
#include "i2c.h"

/**********************************************
 *        MACRO
 **********************************************/
#define CONTROL_REGISTER (0X00)
#define TIMING_REGISTER (0X01)
#define THRESHLOWLOW (0x02)
#define THRESHLOWHIGH (0x03)
#define THRESHHIGHLOW (0x04)
#define THRESHHIGHHIGH (0x05)
#define INTERRUPT (0x06)
#define INDICATION_REGISTER (0x0A)
#define DATA0LOW_REGISTER (0X0C)
#define DATA0HIGH_REGISTER (0X0D)
#define DATA1LOW_REGISTER (0X0E)
#define DATA1HIGH_REGISTER (0X0F)

#define WRITE_COMMAND (0x80)


#define QUEUE_TIMEOUT_TICKS (10)
#define NOTIFY_TAKE_TIMEOUT (500)
#define TEMP_TIME_PERIOD_MS (1000)
#define TEMP_SENSOR_ADDR (0x48)
#define TEMP_REG_OFFSET_ADDR (0x00)
#define LIGHT_SENSOR (0x39)

#define BUFFER (50)

/**********************************************
 *        GLOBALS
 **********************************************/
struct log_struct_temp
{
    char time_stamp[30];
    char temp[40];

};

struct log_struct_led
{
    char time_stamp[30];
    long count;
    char name[10];

};


extern TaskHandle_t handle;

extern uint32_t output_clock_rate_hz;

extern QueueHandle_t myQueue_light;

/**********************************************
 *        Function Prototypes
 **********************************************/
/********************************************
 * Func name :   TempTask
 * Parameters:   none
 * Description : Thread for temperature task
 ********************************************/
void LightTask(void *pvParameters);
/********************************************
 * Func name :   vTimerCallback_Temp_handler
 * Parameters:   none
 * Description : handler for temperature timer
 ********************************************/
void vTimerCallback_Light_handler( TimerHandle_t  *);

/********************************************
 * Func name :   i2c_setup
 * Parameters:   none
 * Description : Configuration of i2c bus
 ********************************************/
void i2c_setup();
void read_lux_CH0();
void read_lux_CH1();
int8_t lux_sensor_setup();
void read_byte_i2c2(uint8_t slave, uint8_t register_addr, uint8_t *data);
void write_byte_i2c2(uint8_t slave, uint8_t register_addr, uint8_t data);
float lux_measurement(float , float );

void I2CSendByte(uint8_t target_address, uint8_t register_address, uint8_t data);
uint8_t I2CGetByte(uint8_t target_address, uint8_t register_address);
#endif /* TEMP_H_ */
