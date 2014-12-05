/***********************************************************************************
 * FILE NAME	: client_main.c

 * DESCRIPTION 	: File contains the main function of the client program which connects
                 to the server for a chat session.
************************************************************************************/

#include "client_header.h"
#include "logger.h"

/***********************************************************************************
 * FUNCTION NAME : main

 * DESCRIPTION 	 : Establishes a client connection with the server, provides for
                   registration/sign-in with a username and password, allows client to
                   enter into a particular group, and then creates seperate threads for
                   reading and writing messages concurrently.

 * RETURNS       : integer(0 on success, 1 on failure)
***********************************************************************************/

int main(int argc, char* argv[])
{
        int sock_desc = 0, ch = 0, ret = 0, flag = 0, verify = 0, count = 0;
        char y = 0, ch1;
        char username[MAX_NAME_LEN];
        char password[MAX_PASSWORD_LEN], tpassword[MAX_PASSWORD_LEN];
        char buffer[MAX_DATA_LEN];
        struct sockaddr_in server;
        pthread_t t_read, t_write;
        CNODE arg;
	

	  //Logger 
	  log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");
        
	  if(2 > argc)
        {
		LOGGER(LOG_CRITICAL,"Enter server IP");
                printf("Enter server IP\n");
                return EXIT_SUCCESS;
        }

        sock_desc = socket(AF_INET, SOCK_STREAM, 0);
        if(0 > sock_desc)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Couldn't create socket");
                perror("Couldn't create socket");
                return EXIT_FAILURE;
        }

        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(argv[1]);
        server.sin_port = htons(PORT);
        printf("Enter your name : ");
        fgets(username, MAX_NAME_LEN, stdin);
        username[strlen(username) - 1] = '\0';
	if(connect(sock_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Connection error");
                perror("Connection error");
                return EXIT_FAILURE;
        }
        printf("\n%s connected to server\n\n", username);
        ret = write(sock_desc, username, strlen(username));
        if(-1 == ret)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Write error");		
                perror("Write error");
                return EXIT_FAILURE;
        }

        //Receive flag to check if the user is registered or not
        receive_flag(sock_desc, &flag);

        if(NEW_USER == flag)
        {
                printf("You are not a registered user. To register, sign up with a password\n\n");
                printf("Enter password(MAX 16 characters) : ");
                scanf("%s", password);
                printf("\n");

        pass:   printf("Re-enter password : ");
                scanf("%s", tpassword);
                printf("\n");

                if(0 == strcmp(password, tpassword))
                {
                        printf("Registration successful\n\n");
                        //Now send password to server
                        ret = write(sock_desc, password, strlen(password));
                        if(-1 == ret)
                        {
				ERROR_LOGGER(LOG_CRITICAL,"Write error");
                                perror("Write error");
                                return EXIT_FAILURE;
                        }
                }
                else
                {
                        printf("Enter correct password\n\n");
                        goto pass;
                }
        }
        if(REGISTERED_USER == flag)
        {
        ver:    printf("Enter password : ");
                scanf("%s", password);
                printf("\n");
                //Send password to server
                ret = write(sock_desc, password, strlen(password));
                if(-1 == ret)
                {
			ERROR_LOGGER(LOG_CRITICAL,"Write error");
                        perror("Write error");
                        return EXIT_FAILURE;
                }
                receive_flag(sock_desc, &verify);
                if(INCORRECT_PASSWORD == verify)
                {
				count++;
                                send_flag(sock_desc, count);
                                if(MAX_LOGIN_ATTEMPTS == count)
                                {
					ERROR_LOGGER(LOG_CRITICAL, "Login attempt failed");
                                        printf("Login attempt failed, exit.\n\n");
					close(sock_desc);
                                        exit(EXIT_FAILURE);
                                }
                                printf("Incorrect password, enter again. \n");
                                goto ver;
                }
                else goto jmp;
        }

		__fpurge(stdin);
        jmp:    printf("List of available chat rooms :\n\n1. Chat room A\n2. Chat room B\n3. Chat room C\n4. Chat room D\n5. Chat room E\n\n");
                printf("Enter your choice : ");
                scanf("%c", &ch1);
		 if ( !(ch1 >=49 && ch1 <=53 ))
                {
                        ERROR_LOGGER(LOG_CRITICAL,"Invaid Input");
                        printf("Invaid Input\n");
                        __fpurge(stdin);
                        goto jmp;
                }
                printf("\n");
                switch(ch1)
                {
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                                send_group_number(sock_desc, ch1);
                                break;
			case '0':
				printf("quit\n");
				exit(0);
                        default:
                                printf("Invalid choice\n\n");
                                //goto jmp;
				//break;
                }
        //Receiving the list of online clients
        ret = read(sock_desc, buffer, MAX_DATA_LEN);
        if(-1 == ret)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Read Error");
                perror("Read error");
                return EXIT_FAILURE;
        }
        buffer[ret] = '\0';

        //Printing the list of online clients
        printf("Online clients :\n");
        printf("%s\n\n", buffer);

join:   printf("Do you want to join this group? (y/n) (Enter 'q' to exit) : ");
        y = getc(stdin);
        y = getc(stdin);
        printf("\n");

        switch(y)
        {
                case 'y':
                        flag = CONFIRMED;
                        send_flag(sock_desc, flag);
                        break;
                case 'n':
                        flag = NOT_CONFIRMED;
                        send_flag(sock_desc, flag);
                        goto jmp;
                        break;
 		case 'q':
                        flag = QUIT;
                        send_flag(sock_desc, flag);
                        printf("Quitting..\n");
                        close(sock_desc);
                        exit(EXIT_SUCCESS);
                        break;
                default:
			ERROR_LOGGER(LOG_CRITICAL,"Invalid choice");
                        printf("Invalid choice, enter either y/n.\n\n");
                        goto join;
        }
        printf("You successfully joined the group\n\n");

        //Initializing the structure to be passed to the read & write threads
        arg.sd = sock_desc;
        strcpy(arg.username, username);
        arg.g_number = ch;

        ret = pthread_create(&t_write, NULL, chat_write, (void*)&arg);
        if(0 != ret)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Thread create error");
                perror("Thread create error");
                return EXIT_FAILURE;
        }

        ret = pthread_create(&t_read, NULL, chat_read, (void*)&arg);
        if(0 != ret)
        {
		ERROR_LOGGER(LOG_CRITICAL,"Thread create error");
                perror("Thread create error");
                return EXIT_FAILURE;
        }

        pthread_join(t_write, NULL);
        pthread_join(t_read, NULL);

        close(sock_desc);
        return EXIT_SUCCESS;
}

