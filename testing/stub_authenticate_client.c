#include"server_header.h"
#include"logger.h"

void stub_call(FILE*,char*);

int main()
{
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

	FILE* fp;
	fp = fopen("repository", "r");
	
	stub_call(fp,"madhu");
	stub_call(fp,"priya");
	stub_call(fp,"ayush");
	stub_call(fp,"nikhil");
	stub_call(fp,"123");
    
	return 0;
}


void stub_call(FILE* fp,char* username)
{

	int head;
	head = authenticate_client(fp,username);

	if(head == 0)
	{	
		printf("\n Registered user\n");	
	}
	else if(head == 1)
	{
		printf("\n New User \n");
	}
	else
	{
		printf("\n EXIT_FAILURE REACHED \n");
	}
}

