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

#define PORT_NO 8003
/*****************************************************************
                        Globals
*****************************************************************/
socklen_t clilen;
struct sockaddr_in to_address;

int new_socket, server_socket;
/**********************************************
       Function for server socket creation
       Parameters : Port number
***********************************************/
int socket_creation_server(int port)
{

        //creating the socket for client 

        server_socket = socket(AF_INET,SOCK_STREAM,0);// setting the client socket
        if(server_socket < 0 ) // enters this loop if port number is not given as command line argument
        {
                //printing error message when opening client socket
                perror("Error opening server socket\n");
                return -1;
        }


        struct sockaddr_in server_address;

        memset(&server_address,0,sizeof(server_address));

        //assigning values for the server address structure
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port); // converting to network byte order
        server_address.sin_addr.s_addr = INADDR_ANY;



        if(bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address))<0)
        {
            perror("Binding failed in the server");
            return -1;
        }

        /*Listening for clients*/
        if(listen(server_socket,10) < 0)
        {
            perror("Error on Listening ");
            return -1;
        }
        else
        {
            printf("\nlistening to remote requests.....\n");
        }

        return 0;

}

/**************************************************
       Function for remote request thread creation
       Parameters : Structure typecasted to void *
***************************************************/
void main()
{
  

  char buffer[10];
    
    
    

    //creating socket for server
    if(socket_creation_server(PORT_NO)== -1)
    {
        perror("Error on socket creation - killed remote request socket");
       
    } 


   while(1)
    {
        new_socket = 0;
        memset(&to_address,0,sizeof(to_address));


        clilen = sizeof(to_address);

        /*accepting client requests*/
        new_socket = accept(server_socket,(struct sockaddr*) &to_address, &clilen);
        if(new_socket<0)
        {
            perror("Error on accepting client");
        }
        else
        {
            printf("established connection\n");
        }

        /*Forked the request received so as to accept multiple clients*/
      int child_id = 0;
        /*Creating child processes*/
        /*Returns zero to child process if there is successful child creation*/
        child_id = fork();

        // error on child process
        if(child_id < 0)
        {
            perror("error on creating child\n");
            exit(1);
        }

        //closing the parent
        if (child_id > 0)
        {
            close(new_socket);
            waitpid(0, NULL, WNOHANG);  //Wait for state change of the child process
        }

        if(child_id == 0)
        {

            memset(buffer,'\0',sizeof(buffer));
            while(recv(new_socket, buffer ,10, 0) > 0)
            {
            
              //printf("Received request %s - %ld\n",buffer,strlen(buffer));

               if(strcmp(buffer,"display")==0)
               {
                    printf("Received request for display\n");

                    char lux[10];
                    memset(lux,'\0',10);
                    strcpy(lux,"100");
                    send(new_socket, lux, 10 , 0);

                    char motion[10];
                    memset(motion,'\0',10);
                    strcpy(motion,"yes");
                    send(new_socket, motion, 10 , 0);

                    char water[10];
                    memset(water,'\0',10);
                    strcpy(water,"23");
                    send(new_socket, water, 10 , 0);
               }
                
            }
            close(new_socket);
            exit(0); 
        }
    }   
}   
