#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "communication.h"

#define PORT_NO 8005

extern pthread_mutex_t lock_res;

pthread_t remote_request_thread;
/**********************************************
       Function for server socket creation
       Parameters : Port number
***********************************************/
int socket_creation_server(int port);


/**************************************************
       Function for remote request thread creation
       Parameters : 
***************************************************/
void *remote_request_callback();

#endif
