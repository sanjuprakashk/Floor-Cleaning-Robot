
#include "spi.h"

struct tx
{
    uint32_t ultrasonic;
    uint32_t water_level;

};

struct tx tx_data;
struct tx rx_data;

void InitSPI()
{

    uint32_t pui32DataTx[3];
    uint32_t pui32DataRx[3];
    int ui32Index;

    // The SSI0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    //enabling PORT A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

       GPIOPinConfigure(GPIO_PA2_SSI0CLK);
       GPIOPinConfigure(GPIO_PA3_SSI0FSS);
       GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
       GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);


       //
       // Configure the GPIO settings for the SSI pins.  This function also gives
       // control of these pins to the SSI hardware.  Consult the data sheet to
       // see which functions are allocated per pin.
       // The pins are assigned as follows:
       //      PA5 - SSI0Tx
       //      PA4 - SSI0Rx
       //      PA3 - SSI0Fss
       //      PA2 - SSI0CLK
       // TODO: change this to select the port/pin you are using.
       //

     GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                          GPIO_PIN_2);




     // Configure and enable the SSI port for SPI MASTER mode.
     SSIConfigSetExpClk(SSI0_BASE, 50000, SSI_FRF_MOTO_MODE_0,
                        SSI_MODE_SLAVE, 1000000, 8);

     //
     // Enable the SSI0 module.
     //
     SSIEnable(SSI0_BASE);

     //flush unwanted data
     while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx[0]))
     {
     }

     //
     // Initialize the data to send.
     //
     pui32DataTx[0] = 's';
     pui32DataTx[1] = 'p';
     pui32DataTx[2] = 'i';

     tx_data.ultrasonic = 10;
     tx_data.water_level = 20;

     //
     // Send 3 bytes of data.
     //

     //
     // Display the data that SSI is transferring.



                 //
                 // Send the data using the "blocking" put function.  This function
                 // will wait until there is room in the send FIFO before returning.
                 // This allows you to assure that all the data you send makes it into
                 // the send FIFO.
                 //
         SSIDataPut(SSI0_BASE, tx_data.ultrasonic);
//         SSIDataPut(SSI0_BASE, tx_data.water_level);

     UARTprintf("Sent SPI data\n");

             //
                 // Wait until SSI0 is done transferring all the data in the transmit FIFO.
                 //
                 while(SSIBusy(SSI0_BASE))
                 {
                 }

                 //
                     // Display indication that the SSI is receiving data.
                     //
//                     UARTprintf("\nReceived:\n  ");

                     //
                     // Receive 3 bytes of data.
                     //

                         //
                         // Receive the data using the "blocking" Get function. This function
                         // will wait until there is data in the receive FIFO before returning.
                         //
//                         SSIDataGet(SSI0_BASE, &rx_data.ultrasonic);
//                         SSIDataGet(SSI0_BASE, &rx_data.water_level);

                         //
                         // Since we are using 8-bit data, mask off the MSB.
                         //

//                         while(SSIBusy(SSI0_BASE))
//                                          {
//                                          }

                         //
                         // Display the data that SSI0 received.
                         //
//                         UARTprintf("R -> '%d' \n", rx_data.water_level);
//                       UARTprintf("R -> '%d' \n", rx_data.water_level);


                     //
                     // Return no errors
                     //



}
