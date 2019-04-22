
#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "timer.h"

#include "driverlib/gpio.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/inc/hw_timer.h"
#include "driverlib/inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "driverlib/inc/hw_ints.h"
#include "driverlib/fpu.h"

void init_motor();

#endif
