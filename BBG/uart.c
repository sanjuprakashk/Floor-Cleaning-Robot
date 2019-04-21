/*Reference : https://github.com/sijpesteijn/BBCLib */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>



typedef struct {
	float lux;
	float temp;
} communication;


typedef enum {
	uart0 = 0, uart1 = 1, uart2 = 2, uart3 = 3, uart4 = 4, uart5 = 5
} uart;


typedef struct {
	int fd;
	uart uart_no;
	int baudrate;
}uart_properties;


int8_t uart_config(uart_properties *uart)
{
	char path[15] = "/dev/ttyO";
	char uart_no[2];
	sprintf(uart_no,"%d",uart->uart_no);
	strcat(path,uart_no);

	struct termios uart_port;

	uart->fd = open(path, O_RDWR | O_NOCTTY | O_SYNC);
	if(uart->fd < 0) printf("port failed to open\n");

	if(!isatty(uart->fd)) perror("Not a tty port\n");


	if(tcgetattr(uart->fd, &uart_port) < 0) 
	{
		perror("Error getting attributes\n");
		return -1;

	}

	uart_port.c_cflag &= ~(CSIZE | PARENB);
	uart_port.c_cflag |= CS8;

	uart_port.c_iflag&= ~(IGNBRK | ICRNL | INLCR | PARMRK | ISTRIP | IXON); //= IGNPAR | ICRNL;
	uart_port.c_oflag = 0;
	uart_port.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

	uart_port.c_cc[VTIME] = 0;
	uart_port.c_cc[VMIN]  = 1;

	fcntl(uart->fd, F_SETFL, O_NONBLOCK);

	if(cfsetispeed(&uart_port, B115200) < 0)
	{
		perror("Input baud rate invalid\n");
		return -1;
	}

   	if(cfsetospeed(&uart_port, B115200) < 0)
   	{
		perror("Output baud rate invalid\n");
		return -1;
	}

	if(tcsetattr(uart->fd,TCSANOW,&uart_port) < 0)
	{
		perror("Attribute setting invalid\n");
		return -1;
	}
	printf("Opened\n");
	return 0;
}

int8_t uart_send(uart_properties *uart, char *tx, int length) {
	if (write(uart->fd, tx, length) == -1) {
		printf("Error in write\n");
		return -1;
	}
	printf("Wrote %c to uart %i\n", *tx, uart->uart_no);
	return 0;
}


int8_t uart_receive(uart_properties *uart, communication *rx, int length) {
	int count = 0;
	if ((count = read(uart->fd, (char *)rx, sizeof(communication))) == -1) {
		printf("Error in read\n");
		return -1;
	}

	printf("Read %f %f from uart %i\n", rx->lux, rx->temp, uart->uart_no);
	return count;
}

int8_t uart_close(uart_properties *uart) {
	close(uart->fd);
	return 0;
}

int main()
{

	communication comm;
	communication comm_rec;

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
	printf("SIZE OF FLOAT = %d", sizeof(float));
	char obj_detect = '1';
	char night_time = '2';
	if (isOpen == 0) {
		unsigned char receive[30];
		while(1)
		{
		//	printf("Entered while\n");
			char buf[30];
			// sprintf(buf, "foo %d", ++i);

/*			comm.lux = 100;
			comm.temp = 23;

			// Send data to uart1
			if (uart_send(uart, &obj_detect, sizeof(communication)) < 0) {
				printf("Could not send data to uart port");
				return -1;
			}

*/
			usleep(1000000);


			comm_rec.lux = 0;
			comm_rec.temp = 0;
			if(uart_receive(uart4,&comm_rec, sizeof(communication)) > 0)
			{
				printf("\n\nLux = %f \t\t Temp = %f\n\n",comm_rec.lux,comm_rec.temp);
				if(comm_rec.temp < 10)
				{
					uart_send(uart, &obj_detect, sizeof(char));
				}

				if(comm_rec.lux < 10)
				{
					uart_send(uart, &night_time, sizeof(char));
				}
			}

			//sprintf(buf,"test uart 1");

			// uart_send(uart, buf, strlen(buf) + 1);
			// usleep(100000);
			// // Read data from uart1
			// if (uart_receive(uart, receive, 30)<  0) {
			// 	printf("Could not read data from uart port");
			// 	return -1;
			// }
		}
		uart_close(uart);
		uart_close(uart4);
	}
	printf("EOF\n");
	return 0;
}


