/***********************************************************************************
 * FILE NAME	: server_main.c

 * DESCRIPTION 	: File contains the main function of the server program which serves
                 client connections.
***********************************************************************************/

#include "server_header.h"
#include "logger.h"

/***********************************************************************************
 * FUNCTION NAME : main

 * DESCRIPTION 	 : Accepts client connections, authenticates clients, maintains the
                   repository, registry, log file and creates a server thread to handle
                   each client concurrently.

 * RETURNS  	 : integer(0 on success, 1 on failure)
***********************************************************************************/

int main(int argc, char* argv[])
{
        struct sockaddr_in server, client;
        int sock_desc = 0, connfd = 0, c = 0, i = 0, yes = 1, ch = 0, ret = 0, flag = 0, verify = 0, count = 0;
        char buffer[MAX_DATA_LEN];
        char username[MAX_NAME_LEN];
        char password[MAX_PASSWORD_LEN];
        char* timestamp = NULL;
        char log_buffer[MAX_DATA_LEN];
        char key[MAX_KEY_LEN];
	FILE *rep = NULL, *reg = NULL;
        pthread_t thread;
        NODE arg;

	//Logger 
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

        //Initializing the log file pointer
        fp = NULL;

        //Initializing structure for signal handling
        struct sigaction sa;
        sa.sa_handler = &handle_signal;
        sa.sa_flags = SA_RESTART;
        sigfillset(&sa.sa_mask);

        //Initializing each group
        for(i =0;i < MAX_GROUPS;i++)
        {
                group[i] = NULL;
        }

        printf("\n-----------------------------------Server started------------------------------------\n\n");
        sock_desc = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_desc < 0)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Couldn't create socket");
                perror("Couldn't create socket");
                return(EXIT_FAILURE);
        }
        printf("Socket created\n\n");
	LOGGER(LOG_CRITICAL, "Socket created");

        if(setsockopt(sock_desc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Error in choosing port\n");
                perror("Setsockopt error");
                exit(EXIT_FAILURE);
        }

        memset(&server, 0, sizeof(server));
        memset(&client, 0, sizeof(client));

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port = htons(PORT);

        if(bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Bind failure");
                perror("Bind failure");
                return EXIT_FAILURE;
        }
        printf("Bind success\n\n");
	LOGGER(LOG_CRITICAL,"Bind success");

        if(listen(sock_desc, BACKLOG) < 0)
        {
		ERROR_LOGGER(LOG_CRITICAL, "Listen failure");
                perror("Listen failure");
                return EXIT_FAILURE;
        }

        c = sizeof(struct sockaddr_in);
        printf("Server waiting for connections...\n\n");
	LOGGER(LOG_CRITICAL, "Server waiting for connections");

        if(-1 == sigaction(SIGINT, &sa, NULL))
        {
	        ERROR_LOGGER(LOG_CRITICAL, "SIGNAL handling failed for SIGINT");
                perror("Error");
                return EXIT_FAILURE;
        }
        if(-1 == sigaction(SIGTSTP, &sa, NULL))
        {
	        ERROR_LOGGER(LOG_CRITICAL,"SIGNAL handling failed for SIGTSTP");
                perror("Error");
                return EXIT_FAILURE;
        }
	while(1)
        {
start:          connfd = accept(sock_desc, (struct sockaddr*)&client, (socklen_t*)&c);
                if(0 > connfd)
                {
			ERROR_LOGGER(LOG_CRITICAL, "Accept Failure");
                        perror("Accept failure");
                        return EXIT_FAILURE;
                }
		LOGGER(LOG_CRITICAL, "Connection accepted");
                timestamp = get_timestamp();
                timestamp[strlen(timestamp) - 1] = '\0';

                //Receiving username from client
                ret = read(connfd, username, MAX_NAME_LEN);
                if(-1 == ret)
                {
			ERROR_LOGGER(LOG_CRITICAL, "Read error");
                        perror("Read error");
                        return EXIT_FAILURE;
                }
                username[ret] = '\0';
		LOGGER(LOG_CRITICAL,"Read username");

                //Search in repository
                flag = authenticate_client(rep, username);

                if(NEW_USER == flag)
                {
                        insert_into_repository(rep, username);
                }

                //Send flag to client
                send_flag(connfd, flag);

                //Now receive the password
        jmp:    ret = read(connfd, password, MAX_PASSWORD_LEN);
                if(-1 == ret)
                {
			ERROR_LOGGER(LOG_CRITICAL, "Read error");
                        perror("Read error");
                        return EXIT_FAILURE;
                }
                password[ret] = '\0';
		LOGGER(LOG_CRITICAL, "Read password");

                //Concatenate username and password into a key
                sprintf(key, "%s%s", username, password);

                if(NEW_USER == flag)
                {
                        //Writing to registry
                        insert_into_registry(reg, key);
                }

                if(REGISTERED_USER == flag)
                {
			//Verify password from the registry
                        verify = verify_password(reg, key);
                        send_flag(connfd, verify);

                        if(INCORRECT_PASSWORD == verify)
                        {
                                receive_flag(connfd, &count);
                                if(MAX_LOGIN_ATTEMPTS == count)
                                {
                                        printf("%s failed to login, disconnecting from server.\n\n", username);
					LOGGER(LOG_CRITICAL,"Maximum login attempts exceeded, disconnecting from server");
                                        close(connfd);
                                        goto start;
                                }
                                else goto jmp;
                        }
                }

                printf("%s with IP address %s connected to server\n\n", username, inet_ntoa(client.sin_addr));

                sprintf(log_buffer, "%s : %s connected to server with IP address %s\n\n", timestamp, username, inet_ntoa(client.sin_addr));

                //Writing to log file
                insert_into_logfile(fp, log_buffer);

                bzero(log_buffer, MAX_DATA_LEN);
                free(timestamp);

                //Receiving client's group choice
        group:  receive_group_number(connfd, &ch);

                //Send list of online clients
                send_client_list(group[ch - 1], connfd);

                //Confirm group choice
                receive_flag(connfd, &flag);
                if(NOT_CONFIRMED == flag)
                        goto group;
                else if(CONFIRMED == flag)
                {
                        printf("%d -> %s joined group %d\n\n", connfd, username, ch);
                }
                else
                {
                        timestamp = get_timestamp();
                        timestamp[strlen(timestamp) - 1] = '\0';
                        printf("%s chose to quit, disconnecting from server.\n\n", username);
                        sprintf(log_buffer, "%s : %s chose to quit, disconnecting from server.\n\n", timestamp, username);

                        //Writing to log file
                        insert_into_logfile(fp, log_buffer);
			LOGGER(LOG_CRITICAL, log_buffer);
                        
			free(timestamp);
			bzero(log_buffer, MAX_DATA_LEN);
                        close(connfd);
			
                        goto start;
                }
                timestamp = get_timestamp();
                timestamp[strlen(timestamp) - 1] = '\0';
                sprintf(log_buffer, "%s : %d -> %s joined group %d\n\n", timestamp, connfd, username, ch);

                //Writing to log file
                insert_into_logfile(fp, log_buffer);
		LOGGER(LOG_CRITICAL, log_buffer);		

                free(timestamp);
                bzero(log_buffer, MAX_DATA_LEN);

                //Inserting client into chosen group
                group[ch - 1] = insert(group[ch - 1], connfd, username);

                printf("Online clients in group %d :\n", ch);
                display(group[ch - 1]);
                printf("\n");

                sprintf(buffer, "%s is online\n", username);
                notify_clients(group[ch - 1], connfd, buffer);
                bzero(buffer, MAX_DATA_LEN);

                //Arguments to the thread handler
                arg.sd = connfd;
                strcpy(arg.username, username);

                //Creating a server thread for each client
                ret = pthread_create(&thread, NULL, thread_server, (void*)&arg);
                if(0 != ret)
                {
			ERROR_LOGGER(LOG_CRITICAL,"Thread create error");
                        perror("Thread create error");
                        return EXIT_FAILURE;
                }
                pthread_detach(thread);
        }
        for(i = 0;i < MAX_GROUPS;i++)
        {
                delete_list(group[i]);
        }
        close(sock_desc);
        return EXIT_SUCCESS;
}

