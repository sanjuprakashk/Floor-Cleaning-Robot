/*
 * temp.c
 *
 *  Created on: Apr 9, 2019
 *      Author: Steve Antony
 */

/**********************************************
 *        Includes
 **********************************************/
#include "temp.h"

/**********************************************
 *        Globals
 **********************************************/
int FLAG_Light = 0;
struct log_struct_temp log_temp;
static char buffer_log[BUFFER];
char temp_buffer[100];

/*for writing and reading as byte from the registers*/
uint8_t register_data;

/*for storing MSB and LSB of CH0 of lux*/
uint16_t MSB_0;
uint16_t LSB_0;

/*for storing MSB and LSB of CH1 of lux*/
uint16_t MSB_1;
uint16_t LSB_1;

/*16 bit value of CH0 and CH1*/
uint16_t CH0;
uint16_t CH1;
float lux_send;

static uint8_t start_again = 1;


/**********************************************
 *         Temperature thread
 **********************************************/

void LightTask(void *pvParameters)
{
    UARTprintf("Created Light Task\n");

        long x_light_id = 10005;
        xTimerHandle xTimer_light;
        xTimer_light = xTimerCreate("Timer_Light",              // Just a text name, not used by the kernel.
                                  pdMS_TO_TICKS( TEMP_TIME_PERIOD_MS ),     // 100ms
                                  pdTRUE,                   // The timers will auto-reload themselves when they expire.
                                  ( void * ) x_light_id,      // Assign each timer a unique id equal to its array index.
                                  vTimerCallback_Light_handler// Each timer calls the same callback when it expires.
                                 );

         if( xTimer_light == NULL )
         {
            // The timer was not created.
            UARTprintf("Error on timer creation\n");
         }
         else
         {
            /*Start led timer*/
            xTimerStart( xTimer_light, 0 );
            i2c_setup();

            lux_sensor_setup();

            for (;;)
            {

                if(FLAG_Light == pdTRUE)
                {
                    FLAG_Light = pdFALSE;
                    read_lux_CH0();
                    read_lux_CH1();

                    lux_send = lux_measurement(CH0,CH1);
                    if(CN_ACTIVE)
                    {
                        xQueueSendToBack( myQueue_light,( void * ) &lux_send, QUEUE_TIMEOUT_TICKS ) ;
                        memset(buffer_log,'\0',BUFFER);
                        sprintf(buffer_log,"L %f\n",lux_send);
                        LOG_INFO(buffer_log)
                    }
                    if((CN_ACTIVE == 0) && (lux_send < 1.0)&&(start_again == 1))
                    {
                        UARTprintf("CN INACTIVE, lux < 1, start\n");
                        forward();
                        start_again = 0;
                        LOG_INFO("Auto on of Robot")
                    }


                }
            }
         }
}


void vTimerCallback_Light_handler( TimerHandle_t  *pxTimer )
{
    FLAG_Light = pdTRUE;
}


void i2c_setup()
{
    /*Enabling i2c pheripheral*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);

    /*Enabling GPIO*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    /*Configuring I2C SDA GPIO*/
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);

    /*Configuring I2C SCL GPIO*/
    GPIOPinConfigure(GPIO_PN5_I2C2SCL);

    /*Configuring ic2 SCL*/
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);

    /*Configuring ic2 SDA*/
    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);

    /*wait till specified peripheral is ready
     * returns true if ready*/
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2));

    /*initiating i2c master*/
    I2CMasterInitExpClk(I2C2_BASE, output_clock_rate_hz, false);
}

void lux_sensor_setup()
{
    /*command to write on control register*/
    register_data = 0x03;
    write_byte_i2c2(LIGHT_SENSOR, CONTROL_REGISTER, register_data);

    /*command to write on TIMING_REGISTER*/
    register_data = 0x12;
    write_byte_i2c2(LIGHT_SENSOR, TIMING_REGISTER, register_data);

}

void read_lux_CH0()
{
    /*command to write on control register*/
    register_data = 0x00;
    read_byte_i2c2(LIGHT_SENSOR, DATA0LOW_REGISTER, &register_data);

    LSB_0 = 0;
    LSB_0 = register_data;

    /*command to write on TIMING_REGISTER*/
    register_data = 0x00;
    read_byte_i2c2(LIGHT_SENSOR, DATA0HIGH_REGISTER, &register_data);

    MSB_0 = 0;
    MSB_0 = register_data;

    /*forming the full 16 bit from MSB and LSB*/
    CH0 = (MSB_0 << 8);
    CH0 |= LSB_0;


}

void read_lux_CH1()
{
    /*command to write on control register*/
    register_data = 0x00;
    read_byte_i2c2(LIGHT_SENSOR, DATA1LOW_REGISTER, &register_data);

    LSB_0 = 0;
    LSB_0 = register_data;

    /*command to write on TIMING_REGISTER*/
    register_data = 0x00;
    read_byte_i2c2(LIGHT_SENSOR, DATA1HIGH_REGISTER, &register_data);

    MSB_1 = 0;
    MSB_1 = register_data;

    /*forming the full 16 bit from MSB and LSB*/
    CH1 = (MSB_1 << 8);
    CH1 |= LSB_1;


}

void read_byte_i2c2(uint8_t slave, uint8_t register_addr, uint8_t * data)
{
    /*select the register to read on slave*/
    I2CMasterSlaveAddrSet(I2C2_BASE, slave, false);

    //command to write to control register
    I2CMasterDataPut(I2C2_BASE, register_addr | WRITE_COMMAND  );

    //Controls the state of the I2C Master , command
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    //Wait until master says it is busy
    while(!I2CMasterBusy(I2C2_BASE));

    //Indicates whether I2C Master is busy
    while(I2CMasterBusy(I2C2_BASE));

    /* reads the data*/
    /*Sets the address that the I2C Master places on the bus*/
     I2CMasterSlaveAddrSet(I2C2_BASE, slave, true);

     I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

     //Wait until master says it is busy
     while(!I2CMasterBusy(I2C2_BASE));

     //Indicates whether I2C Master is busy
     while(I2CMasterBusy(I2C2_BASE));

     *data = I2CMasterDataGet(I2C2_BASE);


}


void write_byte_i2c2(uint8_t slave, uint8_t register_addr, uint8_t data)
{
    /*select the register to read on slave*/

    I2CMasterSlaveAddrSet(I2C2_BASE, slave, false);

    //command to write to control register
    I2CMasterDataPut(I2C2_BASE, register_addr | WRITE_COMMAND  );

    //Controls the state of the I2C Master , command
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    //Wait until master says it is busy
    while(!I2CMasterBusy(I2C2_BASE));

    //Indicates whether I2C Master is busy
    while(I2CMasterBusy(I2C2_BASE));

    I2CMasterDataPut(I2C2_BASE, data);


     I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

     //Wait until master says it is busy
     while(!I2CMasterBusy(I2C2_BASE));

     //Indicates whether I2C Master is busy
     while(I2CMasterBusy(I2C2_BASE));


}


/*****************************************************************
                     Getting lux value
*****************************************************************/
float lux_measurement(float CH0, float CH1)
{

    float ratio = (CH1 / CH0);



    //0 < CH1/CH0 ≤ 0.50 Sensor Lux = (0.0304 x CH0) – (0.062 x CH0 x ((CH1/CH0)1.4))

    if((ratio <=0.5)&& (ratio > 0))
        return ((0.0304 * CH0) - (0.062 * CH0 * (powf(ratio, 1.4))));

    //0.50 < CH1/CH0 ≤ 0.61 Sensor Lux = (0.0224 x CH0) – (0.031 x CH1)

    else if((ratio  > 0.5)&& (ratio <= 0.61))
        return ((0.0224 * CH0) - (0.031 * CH1));

    //0.61 < CH1/CH0 ≤ 0.80 Sensor Lux = (0.0128 x CH0) – (0.0153 x CH1)
    else if((ratio  > 0.61)&& (ratio <= 0.8))
        return (0.0128 * CH0) - (0.0153 * CH1);

    //0.80 < CH1/CH0 ≤ 1.30 Sensor Lux = (0.00146 x CH0) – (0.00112 x CH1)
    else if((ratio  > 0.80)&& (ratio <= 1.30))
        return (0.00146 * CH0) - (0.00112 * CH1);

    //CH1/CH0>1.30 Sensor Lux = 0
    else
        return 0;


}


