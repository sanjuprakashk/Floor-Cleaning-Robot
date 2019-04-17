/*
 * main.h
 *
 *  Created on: Mar 28, 2015
 *      Author: steve
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "driverlib/gpio.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "driverlib/ssi.h"

#include "log.h"

void InitSPI();

extern uint32_t output_clock_rate_hz;

#endif
