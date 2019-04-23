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
typedef struct {
	float lux;
	float distance;
} communication;

typedef enum {
	uart0 = 0, uart1 = 1, uart2 = 2, uart3 = 3, uart4 = 4, uart5 = 5
} uart;


typedef struct {
	int fd;
	uart uart_no;
	int baudrate;
}uart_properties;


int8_t uart_send(uart_properties *uart, void *tx, int length);

int8_t uart_receive(uart_properties *uart, sensor_struct *rx_r, int length);

int8_t uart_close(uart_properties *uart);

int8_t uart_config(uart_properties *uart);

int8_t uart_receive_task(uart_properties *uart, logger_struct *rx_r, int length);
#endif
