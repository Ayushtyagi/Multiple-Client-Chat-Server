/***********************************************************************************
 * FILE NAME: client_header.h
 
 * DESCRIPTION : File contains the header files, function prototypes, labels and 
		 structure definitions used in the files mpcc_client.c and 
		 mpcc_client_functions.c.
***********************************************************************************/

#include        <stdio.h>
#include        <stdlib.h>
#include        <sys/socket.h>                                  //for sockets
#include        <sys/types.h>
#include        <sys/stat.h>
#include        <string.h>                                      //for string operations
#include        <netinet/in.h>                                  //Internet Protocol family sockaddr_in defined here
#include        <pthread.h>                                     //for the cosy POSIX threads
#include        <arpa/inet.h>                                   //for inet_ntoa() function
#include        <unistd.h>                                      //NULL constant defined here
#include        <signal.h>                                      //for ctrl+c & ctrl+z signal
#include        <time.h>
#include        <stdio_ext.h>

#define MAX_NAME_LEN 40
#define MAX_DATA_LEN 256                                        //max size of messages to be sent
#define PORT 2012                                               //default port number
#define MAX_GROUPS 5
#define MAX_PASSWORD_LEN 16
#define NEW_USER 1
#define REGISTERED_USER 0
#define CORRECT_PASSWORD 0
#define INCORRECT_PASSWORD 1
#define MAX_LOGIN_ATTEMPTS 3
#define CONFIRMED 0
#define NOT_CONFIRMED 1
#define QUIT -1

struct cnode
{
        int sd;
        char username[MAX_NAME_LEN];
        int g_number;
};
typedef struct cnode CNODE;

void* chat_write(void*);
void* chat_read(void*);
void send_group_number(int, int);
void send_flag(int, int);
void receive_flag(int, int*);
void handle_signal(int);

