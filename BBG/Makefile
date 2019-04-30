CC = arm-linux-gnueabihf-gcc

CFLAGS = -Iinc/ -Wall -lpthread -lrt -g
all:
	$(CC) src/main.c src/uart.c src/communication.c src/server.c src/logger.c src/heartbeat.c src/log_receiver.c src/POSIX_timer.c $(CFLAGS) -o main

clean:
	rm -f main test.log
