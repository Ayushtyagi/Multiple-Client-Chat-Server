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
	head = fopen("registry", "a");
	
	stub_call(head,"madhu");
	stub_call(head,"priya");
	stub_call(head,"ayush");
	stub_call(head,"$&$\t^");
	stub_call(head,"123");
    
	return 0;
}

void stub_call(FILE* head,char* key)
{

    insert_into_registry(head,key);


    if(head==NULL)
    {
	printf("\n insertion failed\n");	
    }
    else
    {
	printf("\n insertion successful: function working properly \n");
    }
}

