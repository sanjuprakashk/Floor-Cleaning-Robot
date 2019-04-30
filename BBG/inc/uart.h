/**
 * @\file	uart.h
 * @\author	Sanju Prakash Kannioth
 * @\brief	This files contains the declarations and header files for uart transmit and receive on BBG
 * @\date	04/29/2019
 * References : https://github.com/sijpesteijn/BBCLib,
 *				https://en.wikibooks.org/wiki/Serial_Programming/termios
 *
 */

#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>

#include "logger.h"
#include "heartbeat.h"

/* Structure to store sensor data and mode of operation to send to remote request task */
typedef struct {
	float lux;
	float distance;
	float waterLevel;
	int8_t mode;
	int8_t dg_mode;
} communication;

communication comm_rec;

typedef enum {
	uart00 = 0, uart01 = 1, uart02 = 2, uart03 = 3, uart04 = 4, uart05 = 5
} uart;


/* Structure to initialize particular UART on BBG */
typedef struct {
	int fd;
	uart uart_no;
	int baudrate;
}uart_properties;

uart_properties  *uart2;


/**
--------------------------------------------------------------------------------------------
uart_config
--------------------------------------------------------------------------------------------
*   This function will configure the specific UART on BBG
*
*   @\param         uart_properties 	specifies UART number
*
*   @\return        0					success	
*					-1					failure
*
*/
int8_t uart_config(uart_properties *uart);


/**
--------------------------------------------------------------------------------------------
uart_close
--------------------------------------------------------------------------------------------
*   This function will close the specific UART on BBG
*
*   @\param         uart_properties 	specifies UART number
*
*   @\return        0					success
*
*/
int8_t uart_close(uart_properties *uart);


/**
--------------------------------------------------------------------------------------------
uart_send
--------------------------------------------------------------------------------------------
*   This function will send specified length of bytes over the UART on BBG
*
*   @\param         uart_properties 	specifies UART number
*					tx					byte to be sent
*					length				number of bytes to be sent
*
*   @\return        -1					Failure
*					 count				Number of bytes sent
*
*/	
int8_t uart_send(uart_properties *uart, void *tx, int length);


/**
--------------------------------------------------------------------------------------------
uart_receive
--------------------------------------------------------------------------------------------
*   This function will receive tiva sensor data over the UART on BBG
*
*   @\param         uart_properties 	specifies UART number
*					rx_r				byte received
*					length				number of bytes to be received
*
*   @\return        -1					Failure
*					 count				Number of bytes sent
*
*/	
int8_t uart_receive(uart_properties *uart, void *rx_r, int length);


/**
--------------------------------------------------------------------------------------------
uart_receive_task
--------------------------------------------------------------------------------------------
*   This function will receive tiva log data over the UART on BBG
*
*   @\param         uart_properties 	specifies UART number
*					rx_r				byte received
*					length				number of bytes to be received
*
*   @\return        -1					Failure
*					 count				Number of bytes sent
*
*/	
int8_t uart_receive_task(uart_properties *uart, void *rx_r, int length);
#endif
