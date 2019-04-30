# Floor-Cleaning-Robot  

## Obstacle detection task - Ultrasonic distance sensor - HC-SR04  
This is implemented as a FreeRTOS task on the TM4C1294XL board 

**Modes of operation**  
1) Degraded mode: Control node active and Utrasonic sensor dead  
2) Fail-safe mode: Control node inactive and Utrasonic sensor dead  
3) Normal mode: Control node active and Utrasonic sensor is properly connected  
4) Autonomous mode: Control node inactive and Utrasonic sensor is properly connected  

**Actuators**  
1) DC motors driven by a H-bridge IC - L293DNE  

## Automatic wakeup task - Lux sensor - APDS-9301  
This is implemented as a FreeRTOS task on the TM4C1294XL board 

**Modes of operation**  
1) Auto start of robot: The robot will automatically start and run in auto mode when the light in the room is very low(night time) independent of the control node status  

**Actuators**  
1) DC motors driven by a H-bridge IC - L293DNE  

## Water clean task - Water level sensor  
This is implemented as a FreeRTOS task on the TM4C1294XL board  

**Modes of operation**  
1) Auto mode: When the water level is sufficiently high, the valve is opened and when it is low the valve is closed. This works independent of the control node status  

**Actuators**  
1) Solenoid valve driven through a relay module   


## Communication between 2 boards 

** TM4C1294XL **  
There are 3 UARTs being used on the TM4C1294XL. 1 for receiving control node heartbeats, 1 for sending sensor data and another for sending logging data  
Having separate UARTs for each functionality provides the system with the flexibility of working in degraded modes of control even if 1 communication is lost  

** Beaglebone Green **  
There are 3 UARTs being used on the BBG. UART1, UART2 and UART4. 
UART1 is used to receive logging data from Tiva and from other tasks within BBG  
UART2 is used to send control signals and BBG heartbeat signals to the Tiva board  
UART4 is used to receive sensor data from the Tiva board  

## Remote request task  

** Beaglebone Green ** 
This is implemented as a thread on the BBG which reponds to requests from outside and is implemented using TCP sockets. Used to show operational status of the system    
** Supported functionalities **  
Get distance value, lux value, water level value  
Get sensor alive/dead status messages  
Get Tiva board alive/dead status message  
Get valve open/closed status message

Tiva includes - https://rawcdn.githack.com/steveantony5/Floor-Cleaning-Robot/775f34e1458c684fa940017b1a8e5687709b8e34/documentation/Tiva/html/index.html     
BBG includes -  https://rawcdn.githack.com/steveantony5/Floor-Cleaning-Robot/775f34e1458c684fa940017b1a8e5687709b8e34/documentation/BBG/html/index.html  
