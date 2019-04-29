/*Reference : https://github.com/sijpesteijn/BBCLib */

#include "uart.h"


char temp[MAX_BUFFER_SIZE];

struct sensor_struct *rx;

int8_t uart_config(uart_properties *uart)
{
	char path[15] = "/dev/ttyO";
	char uart_no[2];
	sprintf(uart_no,"%d",uart->uart_no);
	strcat(path,uart_no);

	struct termios uart_port;

	uart->fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	if(uart->fd < 0) printf("port failed to open\n");

	if(!isatty(uart->fd)) perror("Not a tty port\n");


	if(tcgetattr(uart->fd, &uart_port) < 0) 
	{
		perror("Error getting attributes\n");
		return -1;

	}

	uart_port.c_cflag &= ~(CSIZE | PARENB);
	uart_port.c_cflag |= CLOCAL | CS8;

	uart_port.c_iflag&= ~(IGNBRK | ICRNL | INLCR | PARMRK | ISTRIP | IXON); //= IGNPAR | ICRNL;
	uart_port.c_oflag = 0;
	uart_port.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

//	uart_port.c_cc[VTIME] = 0;
//	uart_port.c_cc[VMIN]  = 1;

	fcntl(uart->fd, F_SETFL, 0);

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
	return 0;
}


int8_t uart_receive(uart_properties *uart, void *rx_r, int length) {
	int count = 0;
	count = read(uart->fd, rx_r, length);
	if (count  == -1) {
		return -1;
	}

	rx = rx_r;

	if(strcmp(rx->task_name,"DIST") == 0)
	{
		//printf("[%d] DIST = %f\t Mode = %d\n",rx->timeStamp, rx->sensor_data, rx->mode);

		distance_active++;

		comm_rec.distance = rx->distance;

		comm_rec.mode = rx->mode;
		
	}

	else if(strcmp(rx->task_name,"LUX") == 0)
	{
		lux_active++;

		comm_rec.lux = rx->lux;

		comm_rec.mode = rx->mode;

	}
	
	else if(strcmp(rx->task_name,"WAT") == 0)
	{
		water_active++;

		comm_rec.waterLevel = rx->water;
	
		comm_rec.mode = rx->mode;
	}

	else if(strcmp(rx->task_name,"BEA") == 0)
	{
		tiva_active++;
		comm_rec.dg_mode = rx->dg_mode;
	}


	return count;
}

int8_t uart_close(uart_properties *uart) {
	close(uart->fd);
	return 0;
}

int8_t uart_receive_task(uart_properties *uart, void *rx_r, int length) {

	int count = 0;

	count = read(uart->fd, rx_r, length);
	
	if(count == -1)
		return -1;

	memset(temp,'\0', sizeof(temp));
	strcpy(temp,rx_r);
	printf("%s",temp);
	if(count > 1)
	{
		mq_send(msg_queue, temp, MAX_BUFFER_SIZE, 0);
	}

	return 1;
}	
