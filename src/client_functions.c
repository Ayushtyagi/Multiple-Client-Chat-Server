/***********************************************************************************
 * FILE NAME	: server_functions.c
 
 * DESCRIPTION 	: This file contains functions which implement all the functionalities 
		  of the client.
***********************************************************************************/

#include "client_header.h"
#include "logger.h"

/***********************************************************************************
 * FUNCTION NAME : handle_signal
 
 * DESCRIPTION 	 : Handles the SIGTSTP and SIGINT signals
 
 * RETURNS 	 : void
***********************************************************************************/

void handle_signal(int signal)
{
	LOGGER(LOG_CRITICAL,"Entered into handle_signal function");
	int ret = 0;
	char* msg = "Type in 'quit' to exit\n\n";
	ret = write(1, msg, strlen(msg));
	if(-1 == ret)
	{
		ERROR_LOGGER(LOG_CRITICAL,"Write error");
		perror("Write error");
		exit(EXIT_FAILURE);
	}
	LOGGER(LOG_CRITICAL,"Exit from handle_signal function");
}

/***********************************************************************************
 * FUNCTION NAME : send_flag
 
 * DESCRIPTION 	 : Sends a flag/integer to the client.

 * RETURNS 	 : void
***********************************************************************************/

void send_flag(int sd, int flag)
{
	LOGGER(LOG_CRITICAL,"Entered into send_flag function");
        int iflag = 0, ret = 0;
        iflag = htons(flag);
        ret = write(sd, &iflag, sizeof(int));
        if(-1 == ret) 
        {   
		ERROR_LOGGER(LOG_CRITICAL,"Write error");
                perror("Write error");
                exit(EXIT_FAILURE);
        }
	LOGGER(LOG_CRITICAL,"Exit from send_flag function");   
}

/***********************************************************************************
 * FUNCTION NAME : receive_flag
 
 * DESCRIPTION 	 : Receives a flag/integer from the client.

 * RETURNS 	 : void
***********************************************************************************/

void receive_flag(int sd, int *flag)
{
	LOGGER(LOG_CRITICAL,"Entered into receive_flag function");
	int iflag = 0, ret = 0;
	ret = read(sd, &iflag, sizeof(int));
	if(-1 == ret)
	{
		ERROR_LOGGER(LOG_CRITICAL,"Read error");
		perror("Read error");
		exit(EXIT_FAILURE);
	}
	*flag = ntohs(iflag);
	LOGGER(LOG_CRITICAL,"Exit from receive_flag function");
}

/***********************************************************************************
 * FUNCTION NAME : send_group_number
 
 * DESCRIPTION 	 : Sends the group number chosen by the client, to the server.
 
 * RETURNS 	 : void
***********************************************************************************/

void send_group_number(int sd, int num)
{
	LOGGER(LOG_CRITICAL,"Entered into send_group_number function");
        int inum = 0, ret = 0;
        inum = htons(num);
        ret = write(sd, &inum, sizeof(int));
        if(-1 == ret)
	{
		ERROR_LOGGER(LOG_CRITICAL,"Write error");
		perror("Write error");
                exit(EXIT_FAILURE);
	}
	LOGGER(LOG_CRITICAL,"Exit from send_group_number function");
}

/***********************************************************************************
 * FUNCTION NAME : chat_read
 
 * DESCRIPTION 	 : Reads all incoming messages from the server and messages sent by 
		   other clients present in the same chat group. These messages are
		   then printed on the client's screen.

 * RETURNS 	 : void*
***********************************************************************************/

void* chat_read(void* arg)
{
	LOGGER(LOG_CRITICAL,"Entered into chat_read function");
        int ret = 0, sd = 0;
        char buffer[MAX_DATA_LEN];
	
        struct cnode *ptr = (struct cnode*)arg;
        sd = ptr->sd;
	
	//Initializing structure for signal handling
        struct sigaction sa;
        sa.sa_handler = &handle_signal;
        sa.sa_flags = SA_RESTART;
        sigfillset(&sa.sa_mask);

        if(-1 == sigaction(SIGINT, &sa, NULL))
        {
	        ERROR_LOGGER(LOG_CRITICAL, "SIGNAL handling failed for SIGINT");
		perror("Error");
		exit(EXIT_FAILURE);
	}

        if(-1 == sigaction(SIGTSTP, &sa, NULL))
        {
	        ERROR_LOGGER(LOG_CRITICAL, "SIGNAL handling failed for SIGTSTP");
	        perror("Error");
		exit(EXIT_FAILURE);
	}

	while(1)
        {
                ret = read(sd, buffer, MAX_DATA_LEN);
		buffer[ret] = '\0';

                if(-1 == ret)
		{
			ERROR_LOGGER(LOG_CRITICAL,"Write error");
			perror("Read error");
			exit(EXIT_FAILURE);
                }
		if(0 == ret)
		{
			close(sd);
			LOGGER(LOG_CRITICAL,"Exit from chat_read function");
			printf("Quitting..\n");
			exit(EXIT_SUCCESS);
//			pthread_exit(NULL);
		}
                if(0 < ret)
                {
			printf("%s\n", buffer);
                }
	        bzero(buffer, MAX_DATA_LEN);
        }
	LOGGER(LOG_CRITICAL,"Exit from chat_read function");
}

/***********************************************************************************
 * FUNCTION NAME : chat_write
 
 * DESCRIPTION 	 : This functions allows a client to type in messages to be sent to 
		   other clients present in the chat group. If the client enters 'quit'
		   , it exits the chat.

 * RETURNS 	 : void*
***********************************************************************************/

void* chat_write(void* arg)
{
	LOGGER(LOG_CRITICAL,"Entered into chat_write function");
        int ret = 0, sd = 0;
        char buffer[MAX_DATA_LEN];
	int g_number = 0;

        struct cnode *ptr = (struct cnode*)arg;
        sd = ptr->sd;
	g_number = ptr->g_number;

	//Sending the group number to server
	send_group_number(sd, g_number);

	printf("Chat session (enter 'quit' to exit) :");

        while(1)
        {
		printf("\n");
                fgets(buffer, MAX_DATA_LEN, stdin);
                buffer[strlen(buffer) - 1] = '\0';

                if(strlen(buffer) > (MAX_DATA_LEN - 1))
                {
                        printf("Buffer size full,\t enter within %d characters\n", MAX_DATA_LEN);
                        bzero(buffer, MAX_DATA_LEN);
                        __fpurge(stdin);
                }

                ret = write(sd ,buffer, strlen(buffer));
		if(-1 == ret)
		{
	        	ERROR_LOGGER(LOG_CRITICAL, "Write error");
			perror("Write error");
			exit(EXIT_FAILURE);
		}
   
                if(0 == strcmp(buffer, "quit"))
                {
			close(sd);
			LOGGER(LOG_CRITICAL,"Exit from chat_write function");  
			printf("Quitting..\n");
//		        pthread_exit(NULL);
			exit(EXIT_SUCCESS);
		}
                bzero(buffer, MAX_DATA_LEN);
        }//while ends
	LOGGER(LOG_CRITICAL,"Exit from chat_write function");  
}

