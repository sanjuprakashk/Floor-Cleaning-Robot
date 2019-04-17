/*Reference : https://github.com/sijpesteijn/BBCLib */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>


typedef enum {
	uart0 = 0, uart1 = 1, uart2 = 2, uart3 = 3, uart4 = 4, uart5 = 5
} uart;


typedef struct {
	int fd;
	uart uart_no;
	int baudrate;
}uart_properties;


uint8_t uart_config(uart_properties *uart)
{
	char path[15] = "/dev/ttyO";
	char uart_no[2];
	sprintf(uart_no,"%d",uart->uart_no);
	strcat(path,uart_no);

	struct termios uart_port;

	uart->fd = open(path, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
	if(uart->fd < 0) printf("port failed to open\n");

	uart_port.c_cflag = uart->baudrate | CS8 | CLOCAL | CREAD;
	uart_port.c_iflag&= ~(IGNBRK | ICRNL | INLCR | PARMRK | ISTRIP | IXON); //= IGNPAR | ICRNL;
	uart_port.c_oflag = 0;
	uart_port.c_lflag = 0;

	uart_port.c_cc[VTIME] = 0;
	uart_port.c_cc[VMIN]  = 1;

	cfsetispeed(&uart_port, B115200);
   	cfsetospeed(&uart_port, B115200);

	tcsetattr(uart->fd,TCSAFLUSH,&uart_port);
	printf("Opened\n");
	return 0;
}

int8_t uart_send(uart_properties *uart, char *tx, int length) {
	if (write(uart->fd, tx, length) == -1) {
		printf("Error in write\n");
		return -1;
	}
	printf("Wrote %s to uart %i\n", tx, uart->uart_no);
	return 0;
}


int8_t uart_receive(uart_properties *uart, char *rx, int length) {
	if (read(uart->fd, rx, length) == -1) {
		printf("Error in write\n");
		return -1;
	}

	printf("Read %s from uart %i\n", rx, uart->uart_no);
	return 0;
}

int8_t uart_close(uart_properties *uart) {
	close(uart->fd);
	return 0;
}

int main()
{

	uart_properties *uart  = malloc(sizeof(uart_properties));
	uart->uart_no = uart1;
	uart->baudrate = B115200;

	uart_properties *uart4 = malloc(sizeof(uart_properties));
	uart4->uart_no = 4;
	uart4->baudrate = B115200;

	uint8_t isOpen = uart_config(uart);
	uint8_t isOpen4 = uart_config(uart4);
	int i = 0;
	printf("Open success? %d\n", isOpen);
	if (isOpen == 0) {
		unsigned char receive[30];
		while(1)
		{
		//	printf("Entered while\n");
			char buf[30];
			sprintf(buf, "foo %d", ++i);

			// Send data to uart1
			if (uart_send(uart4, buf, strlen(buf) + 1) < 0) {
				printf("Could not send data to uart port");
				return -1;
			}


			usleep(100000);

			uart_receive(uart4,receive,30);
                        sprintf(buf,"test uart 1");
			uart_send(uart, buf, strlen(buf) + 1);
			usleep(100000);
			// Read data from uart1
			if (uart_receive(uart, receive, 30)<  0) {
				printf("Could not read data from uart port");
				return -1;
			}
		}
		uart_close(uart);
		uart_close(uart4);
	}
	printf("EOF\n");
	return 0;
}

