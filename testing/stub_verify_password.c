#include"server_header.h"
#include"logger.h"

void stub_call(FILE*,char*);

int main()
{
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

	FILE* head;
	head = fopen("registry", "r");
	
	stub_call(head,"madhu");
	stub_call(head,"priya");
	stub_call(head,"123");
	stub_call(head,"\t");
	stub_call(head,"abc123");
    
	return 0;
}

void stub_call(FILE* head,char* username)
{
	int ret=0;
	ret = verify_password(head,username);

	if(ret== 1)
	{	
		printf("\n Incorrect password entry\n");	
	}
	else if(ret == 0)
	{
		printf("\n Correct password entry \n");
	}
	else
	{
		printf("\n authentication successful: function working properly \n");
	}
}

