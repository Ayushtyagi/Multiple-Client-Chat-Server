#include"server_header.h"
#include"logger.h"

void stub_call(NODE*);

int main()
{
	 log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

	NODE* head = NULL;	

	stub_call(head);
    
	return 0;
}





void stub_call(NODE* head)
{
       display(head);

    if(head==NULL)
    {
	printf("\n deletion of node failed\n");	
    }
    else
    {
	printf("\n deletion of node successful: function working properly \n");
    }
}

