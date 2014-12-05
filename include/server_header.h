/***********************************************************************************
 * FILE NAME: server_header.h

 * DESCRIPTION : File contains the header files, function prototypes, global variables,
                 labels and structure definitions used in the files mpcc_server.c and
                 mpcc_server_functions.c.

        DATE            NAME            REFERENCE               REASON
      23/Nov/14     Nikhil Fadnis                               Project

 * Copyright 2014 , Aricent Technologies (Holdings) Ltd
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

#define MAX_NAME_LEN 30
#define BACKLOG 100                                             //connections in the queue
#define MAX_DATA_LEN 256                                        //max size of messages to be sent
#define PORT 2012                                               //default port number
#define MAX_GROUPS 5
#define MAX_PASSWORD_LEN 16
#define MAX_KEY_LEN 46
#define NEW_USER 1
#define REGISTERED_USER 0
#define INCORRECT_PASSWORD 1
#define CORRECT_PASSWORD 0
#define NOT_CONFIRMED 1
#define CONFIRMED 0
#define MAX_LOGIN_ATTEMPTS 3
#define QUIT -1

FILE *fp;

struct node                                                     //structure definition
{
        int sd;
        char username[MAX_NAME_LEN];
        struct node *next;
};
typedef struct node NODE;
NODE* group[MAX_GROUPS];                                        //declaring an array of structures of type NODE

void display(NODE*);
NODE* insert(NODE*, int, char*);
void notify_clients(NODE*, int, char*);
NODE* delete_node(NODE*, int);
NODE* delete_list(NODE*);
void* thread_server(void* arg);
void receive_group_number(int, int*);
char* get_timestamp();
void send_flag(int, int);
void receive_flag(int, int*);
int authenticate_client(FILE*, char*);
int verify_password(FILE*, char*);
void insert_into_repository(FILE*, char*);
void insert_into_registry(FILE*, char*);
void insert_into_logfile(FILE*, char*);
void send_client_list(NODE*, int);
void handle_signal(int);

