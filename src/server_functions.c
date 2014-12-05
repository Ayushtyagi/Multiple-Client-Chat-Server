/***********************************************************************************
 * FILE NAME   : mpcc_server_functions.c

 * DESCRIPTION : This file contains functions which implement all the functionalities
                 of the server.
***********************************************************************************/

#include "server_header.h"
#include "logger.h"

/***********************************************************************************
 * FUNCTION NAME : authenticate_client

 * DESCRIPTION 	 : Checks whether a client is a registered user or a new user.

 * RETURNS       : integer
***********************************************************************************/

int authenticate_client(FILE* repository, char* username)
{
	LOGGER(LOG_CRITICAL,"Entered into authenticate_client function");
        char buffer[MAX_NAME_LEN];
        repository = fopen("repository", "r");
        if(NULL == repository)
        {	
		ERROR_LOGGER(LOG_CRITICAL,"Error opening repository");
                perror("Error opening repository");
                exit(EXIT_FAILURE);
        }
        while(NULL != fgets(buffer, MAX_NAME_LEN, repository))
        {
                buffer[strlen(buffer) - 1] = '\0';
                if(0 == strcmp(username, buffer))
                {
                        fclose(repository);
			LOGGER(LOG_CRITICAL,"Exit from authenticate_client function");        
                        return REGISTERED_USER;
                }
        }
        fclose(repository);
	LOGGER(LOG_CRITICAL,"Exit from authenticate_client function");        
	return NEW_USER;
}

/***********************************************************************************
 * FUNCTION NAME : insert_into_logfile

 * DESCRIPTION 	 : Writes the log buffer into the log file.

 * RETURNS 	 : void
***********************************************************************************/
void insert_into_logfile(FILE* fp, char* buffer)
{
	LOGGER(LOG_CRITICAL,"Entered into insert_into_logfile function");
        fp = fopen("log_file", "a");
        if(NULL == fp)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Error opening log file");
                perror("Error opening log file");
                exit(EXIT_FAILURE);
        }
        fputs(buffer, fp);
        fclose(fp);
	LOGGER(LOG_CRITICAL,"Exit from insert_into_logfile function");
}

/***********************************************************************************
 * FUNCTION NAME : insert_into_repository

 * DESCRIPTION 	 : Writes the username of a client into the repository.

 * RETURNS 	 : void
***********************************************************************************/

void insert_into_repository(FILE* repository, char* username)
{
	LOGGER(LOG_CRITICAL,"Entered into insert_into_repository function");
        repository = fopen("repository", "a");
        if(NULL == repository)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Error opening repository");
                perror("Error opening repository");
                exit(EXIT_FAILURE);
        }
        fprintf(repository, "%s\n", username);
        fclose(repository);
	LOGGER(LOG_CRITICAL,"Exit from insert_into_repository function");
}

/***********************************************************************************
 * FUNCTION NAME : insert_into_registry

 * DESCRIPTION 	 : Writes the username & password (key) of a client into the repository.

 * RETURNS 	 : void
***********************************************************************************/

void insert_into_registry(FILE* registry, char* key)
{
	LOGGER(LOG_CRITICAL,"Entered into insert_into_registry function");
        registry = fopen("registry", "a");
        if(NULL == registry)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Error opening registry");
                perror("Error opening registry");
                exit(EXIT_FAILURE);
        }
        fprintf(registry, "%s\n", key);
        fclose(registry);
	LOGGER(LOG_CRITICAL,"Exit from insert_into_registry function");
}

/***********************************************************************************
 * FUNCTION NAME : verify_password

 * DESCRIPTION 	 : Verifies the password entered by a user.

 * RETURNS 	 : integer
***********************************************************************************/

int verify_password(FILE* registry, char* key)
{
	LOGGER(LOG_CRITICAL,"Entered into verify_password function");
	char buffer[MAX_KEY_LEN];
	
	registry = fopen("registry", "r");        
	if(NULL == registry)
	{
		ERROR_LOGGER(LOG_CRITICAL, "Error opening registry");
		perror("Error opening registry");
		exit(EXIT_FAILURE);
	}
        
	while(fgets(buffer, MAX_KEY_LEN, registry) != NULL)
        {
                buffer[strlen(buffer) - 1] = '\0';
                if(0 == strcmp(key, buffer))
                {
			LOGGER(LOG_CRITICAL,"Exit from verify_password function");
                        fclose(registry);
			return CORRECT_PASSWORD;
                }
        }
	LOGGER(LOG_CRITICAL,"Exit from verify_password function");
	fclose(registry);
        return INCORRECT_PASSWORD;
}

/***********************************************************************************
 * FUNCTION NAME : handle_signal

 * DESCRIPTION 	 : Handles the SIGTSTP and SIGINT signals

 * RETURNS 	 : void
***********************************************************************************/

void handle_signal(int signal)
{
	LOGGER(LOG_CRITICAL,"Entered into handle_signal function");
        int ret = 0, i = 0;
        NODE *cur = NULL;
        char* msg = "Server shutdown\n\n";
        ret = write(1, msg, strlen(msg));
        if(ret == -1)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Write error");
                perror("Write error");
                exit(EXIT_FAILURE);
        }
        msg = "Server down, closing clients..\n\nDue to some unexpected reason, the server has shut down, exit.\n";
        for(i = 0;i < MAX_GROUPS;i++)
        {
                cur = group[i];
                while(cur != NULL)
                {
                        ret = write(cur->sd, msg, strlen(msg));
                        if(ret == -1)
                        {
				ERROR_LOGGER(LOG_CRITICAL,"Write error");
                                perror("Write error");
                                exit(EXIT_FAILURE);
                        }
                        cur = cur->next;

                }
        }
	LOGGER(LOG_CRITICAL,"Exit from handle_signal function");
        exit(EXIT_SUCCESS);
}

/***********************************************************************************
 * FUNCTION NAME : receive_flag

 * DESCRIPTION : Receives a flag/integer from the client.

 * RETURNS : void
***********************************************************************************/

void receive_flag(int sd, int *flag)
{
	LOGGER(LOG_CRITICAL,"Entered into receive_flag function");
        int iflag = 0, ret = 0;
        ret = read(sd, &iflag, sizeof(int));
        if(ret == -1)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Read error");
                perror("Read error");
                exit(EXIT_FAILURE);
        }
        *flag = ntohs(iflag);
	LOGGER(LOG_CRITICAL,"Exit from receive_flag function");
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
        if(ret == -1)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Write error");
                perror("Write error");
                exit(EXIT_FAILURE);
        }
	LOGGER(LOG_CRITICAL,"Exit from send_flag function");   
}

/***********************************************************************************
 * FUNCTION NAME : receive_group_number

 * DESCRIPTION 	 : Receives the group number chosen by the client.

 * RETURNS 	 : void
***********************************************************************************/

void receive_group_number(int sd, int* num)
{
	LOGGER(LOG_CRITICAL,"Entered into receive_group_number function");
        int inum = 0, ret = 0;
        ret = read(sd, &inum, sizeof(int));
        if(ret == -1)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Read error");
                perror("Read error");
                exit(EXIT_FAILURE);
        }
        *num = ntohs(inum);
	LOGGER(LOG_CRITICAL,"Exit from receive_group_number function");
}

/***********************************************************************************
 * FUNCTION NAME : notify_clients

 * DESCRIPTION 	 : Broadcasts a message to all clients in a group.

 * RETURNS 	 : void
***********************************************************************************/

void notify_clients(NODE* first, int sd, char* buffer)
{
	LOGGER(LOG_CRITICAL,"Entered into notify_clients function");
        int ret = 0;
	NODE* cur = NULL;
        if(first == NULL)
                return;
        cur = first;
        while(cur != NULL)
        {
                if(sd != cur->sd)
		{
                        ret = write(cur->sd, buffer, strlen(buffer));
			if(ret == -1)
			{
				ERROR_LOGGER(LOG_CRITICAL,"Write error");
				perror("Write error");
				exit(EXIT_FAILURE);
			}
		}
                cur = cur->next;
        }
	LOGGER(LOG_CRITICAL,"Exit from notify_clients function");
}

/***********************************************************************************
 * FUNCTION NAME : insert

 * DESCRIPTION 	 : Inserts a new client into a group.

 * RETURNS 	 : NODE* (first/head node of the group)
***********************************************************************************/

NODE* insert(NODE *first, int sd, char* username)
{
        LOGGER(LOG_CRITICAL,"Entered into insert function");
        NODE *temp = NULL;
        temp = malloc(sizeof(struct node));
        if(temp == NULL)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Malloc error");
                perror("Memory not available");
                return first;
        }
        temp->sd = sd;
        strcpy(temp->username, username);
        temp->next = first;
        LOGGER(LOG_CRITICAL,"Exit from insert function");
        return temp;
}

/***********************************************************************************
 * FUNCTION NAME : thread_server

 * DESCRIPTION 	 : Each thread_server function recieves messages sent by a client
                   and broadcasts it all clients in a group. If the message is a
                   'quit' request, it deletes the client from the group, notifies other
                   clients that the user has left the chat, and updates the log file.

 * RETURNS 	 : void*
***********************************************************************************/

void* thread_server(void *arg)
{
        LOGGER(LOG_CRITICAL,"Entered into thread_server function");
        int sd = 0, ret = 0, ch = 0;
        char username[MAX_NAME_LEN], buffer[MAX_DATA_LEN];
        struct node *ptr = (struct node*)arg;
        char *timestamp = NULL;
        char log_buffer[MAX_DATA_LEN];
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	char *strp = NULL;
        char final_msg[MAX_DATA_LEN];
	char *msg = malloc(MAX_DATA_LEN);
	if(msg == NULL)
	{
		ERROR_LOGGER(LOG_CRITICAL,"Malloc error");
		perror("Malloc error");
		exit(EXIT_FAILURE);
	}
        sd = ptr->sd;
        strcpy(username, ptr->username);

        receive_group_number(sd, &ch);

        //Start chatting
        while(1)
        {
                ret = read(sd, buffer, MAX_DATA_LEN);
                buffer[ret] = '\0';

                if(ret == 0)
                        goto jmp;
                //If a client quits
                if(strncmp(buffer, "quit", 4) == 0)
                {
                jmp:    printf("%d -> %s leaving chat, deleting from list\n", sd, username);
                        //bzero(buffer, MAX_DATA_LEN);
                        timestamp = get_timestamp();
                        timestamp[strlen(timestamp) - 1] = '\0';
                        sprintf(log_buffer, "%s : %s has left the chat\n\n", timestamp, username);

                        free(timestamp);

                        //Mutex lock, update log file
                        pthread_mutex_lock(&mutex);
                        fp = fopen("log_file", "a");
                        if(fp == NULL)
                        {
				ERROR_LOGGER(LOG_CRITICAL, "Error opening log file");
                                perror("Error opening log file");
                                pthread_exit(NULL);
                        }
                        fputs(log_buffer, fp);
                        bzero(log_buffer, MAX_DATA_LEN);
                        fclose(fp);
                        //Mutex unlock
                        pthread_mutex_unlock(&mutex);

                        sprintf(buffer, "%s has left the chat\n", username);
                        notify_clients(group[ch - 1], sd, buffer);
                        bzero(buffer, MAX_DATA_LEN);
                        group[ch - 1] = delete_node(group[ch - 1], sd);
                        close(sd);
                        free(msg);
			LOGGER(LOG_CRITICAL,"Exit from thread_server function");
                        break;
                }

                printf("%s : %s\n\n",username, buffer);

                strcpy(msg, username);
                strp = msg;
                strp += strlen(msg);
                strcat(strp, buffer);

                sprintf(final_msg, "%s : %s\n", username, strp);

                notify_clients(group[ch - 1], sd, final_msg);

                bzero(buffer, MAX_DATA_LEN);
                bzero(msg, MAX_DATA_LEN);
                bzero(final_msg, MAX_DATA_LEN);
        }
	LOGGER(LOG_CRITICAL,"Exit from thread_server function");
        pthread_exit(NULL);
}

/***********************************************************************************
 * FUNCTION NAME : delete_node

 * DESCRIPTION 	 : Removes a client/user from a particular group.

 * RETURNS 	 : NODE* (first/head node of the group)
***********************************************************************************/

NODE* delete_node(NODE* first, int sd)
{
	LOGGER(LOG_CRITICAL,"Entered into delete_node function");
        NODE *prev = NULL, *cur = NULL;
        if(first == NULL)
        {
                return first;
        }
        if(first->next == NULL)
        {
                if(sd == first->sd)
                {
                        free(first);
                        return NULL;
		}
                else
                        return first;
        }
        cur = first;
        while(cur != NULL)
        {
                if(sd == first->sd)
                {
                        first = first->next;
                        free(cur);
			break;
                }
                prev = cur;
                cur = cur->next;
                if(sd == cur->sd)
                {
                        prev->next = cur->next;
                        free(cur);
			break;
                }
        }
	LOGGER(LOG_CRITICAL,"Exit from delete_node function");
        return first;
}

/***********************************************************************************
 * FUNCTION NAME : display

 * DESCRIPTION : Displays all the clients online in a particular group.

 * RETURNS : void
***********************************************************************************/

void display(NODE* first)
{
	LOGGER(LOG_CRITICAL,"Entered into display function");
        NODE *cur = NULL;
        if(first == NULL)
        {
                printf("No clients online\n");
		LOGGER(LOG_CRITICAL,"Exit from display function");
                return;
        }
        cur = first;
        while(cur != NULL)
        {
                printf("%d -> %s\t", cur->sd, cur->username);
                cur = cur->next;
        }
        printf("\n");
	LOGGER(LOG_CRITICAL,"Exit from display function");
}

/***********************************************************************************
 * FUNCTION NAME : delete_list

 * DESCRIPTION : Removes all the clients from a particular group.

 * RETURNS : NODE*
***********************************************************************************/

NODE* delete_list(NODE* first)
{
	LOGGER(LOG_CRITICAL,"Enter into delete_list function");
        NODE *temp = NULL;
        if(first == NULL)
	{
		LOGGER(LOG_CRITICAL,"Exit from delete_list function");
                return NULL;
	}
        while(first != NULL)
        {
                temp = first;
                first = first->next;
                free(temp);
        }
	LOGGER(LOG_CRITICAL,"Exit from delete_list function");
        return NULL;
}

/***********************************************************************************
 * FUNCTION NAME : get_timestamp

 * DESCRIPTION : Returns the timestamp as a string

 * RETURNS : char*
***********************************************************************************/

char* get_timestamp()
{
	LOGGER(LOG_CRITICAL,"Enter into get_timestamp function");
        char *timestamp = NULL;
        time_t ltime;
        ltime = time(NULL);
        timestamp = strdup(asctime(localtime(&ltime)));
	LOGGER(LOG_CRITICAL,"Exit from get_timestamp function");
        return timestamp;
}

/***********************************************************************************
 * FUNCTION NAME : send_client_list

 * DESCRIPTION : Sends a list of online users in a particular group to the client.

 * RETURNS : void
***********************************************************************************/

void send_client_list(NODE* first, int sd)
{
	LOGGER(LOG_CRITICAL,"Enter into send_client_list function");
        int ret = 0;
        NODE* cur = NULL;
        char buffer[MAX_DATA_LEN];
        char *msg = "No clients online";

        if(first == NULL)
        {
                ret = write(sd, msg, strlen(msg));
                if(ret == -1)
                {
			ERROR_LOGGER(LOG_CRITICAL,"Write error");
                        perror("Write error");
                        exit(EXIT_FAILURE);
                }
        }

        cur = first;
        while(cur != NULL)
        {
                sprintf(buffer, "%s\t", cur->username);
                ret = write(sd, buffer, strlen(buffer));
                if(ret == -1)
                {
			ERROR_LOGGER(LOG_CRITICAL,"Write error");
                        perror("Write error");
                        exit(EXIT_FAILURE);;
                }
                cur = cur->next;
        }
	LOGGER(LOG_CRITICAL,"Exit from send_client_list function");
}

