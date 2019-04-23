/*Reference : https://github.com/sijpesteijn/BBCLib */

#include "uart.h"


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
	uart_port.c_cc[VMIN]  = 100;

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

int8_t uart_send(uart_properties *uart, void *tx, int length) {
	int count = write(uart->fd, tx, length);
	if (count == -1) {
		printf("Error in write\n");
		return -1;
	}
	
	/*printf("Size of write = %d\n", count);
	struct sensor_struct *tx_r = tx;
	printf("Wrote %s %d %f to uart %i\n",tx_r->task_name, tx_r->timeStamp, tx_r->sensor_data, uart->uart_no);
	*/
	return 0;
}


int8_t uart_receive(uart_properties *uart,void  *rx_r, int length) {
	int count = 0;
	count = read(uart->fd, rx_r, length);
	if (count  == -1) {
	//	printf("Error in read\n");
		return -1;
	}

	printf("Size of read = %d \t\t Size of length = %d\n", count, length);
	
	struct logger_struct *rx_log = rx_r;
	printf("TASK NAME = %s\n", rx_log->task_name);

	if(strcmp(rx_log->task_name,"LUX") == 0)
	{
		struct sensor_struct *rx = rx_r;
		printf("[%d] LUX = %f\n",rx->timeStamp, rx->sensor_data);
		
	}

	else if(strcmp(rx_log->task_name,"DIST") == 0)
	{
		struct sensor_struct *rx = rx_r;
		printf("[%d] DIST = %f\n",rx->timeStamp, rx->sensor_data);

	}
	
	else if(strcmp(rx_log->task_name,"WATER") == 0)
	{
		struct sensor_struct *rx = rx_r;
		printf("[%d] WATER LEVEL = %f\n",rx->timeStamp, rx->sensor_data);
	}

	else if(strcmp(rx_log->task_name,"LOG") == 0)
	{
		struct logger_struct *rx_log1 = rx_r;
		printf("[%d] LOG MESSAGE = %s\n", rx_log1->timeStamp, rx_log1->log);
	}

	//printf("Read %s %d %f from uart %i\n", rx->task_name, rx->timeStamp,rx->sensor_data, uart->uart_no);
	return count;
}

int8_t uart_close(uart_properties *uart) {
	close(uart->fd);
	return 0;
}

int8_t *uart_receive_task(uart_properties *uart, void *rx_r, int length) {
	int count = 0;
	count = read(uart->fd, rx_r, length);
	
//	memset(task_name,'\0',sizeof(task_name));	
//	strcpy(task_name, rx_r);

	return 0;
}	
