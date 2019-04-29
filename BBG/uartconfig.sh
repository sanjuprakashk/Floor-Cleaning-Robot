#!/bin/sh
    echo "\n Pin configuration available" >> uartconf.txt
    echo "\n UART 4 configuration p9.11 and p9.13"
    sudo config-pin P9.11 uart
    sudo config-pin -q P9.11
    sudo config-pin P9.13 uart
    sudo config-pin -q P9.13
   
    echo "\n UART 1 configuration p9.26 and p9.24"
    sudo config-pin P9.24 uart
    sudo config-pin -q P9.24
    sudo config-pin P9.26 uart
    sudo config-pin -q P9.26

    echo "\n UART 2 configuration p9.22 and p9.21"
    sudo config-pin P9.22 uart
    sudo config-pin -q P9.22
    sudo config-pin P9.21 uart
    sudo config-pin -q P9.21
