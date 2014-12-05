#include"server_header.h"
#include"logger.h"

void stub_call(NODE*,int);

int main()
{
	 log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

	NODE* head = NULL;
	int ab=98;	

	stub_call(head,5);
	stub_call(head,3);
	stub_call(head,ab);
	stub_call(head,"madhu");
    
	return 0;
}





void stub_call(NODE* head,int fd)
{
       head = delete_node(head,fd);


    if(head==NULL)
    {
	printf("\n deletion of node failed\n");	
    }
    else
    {
	printf("\n deletion of node successful: function working properly \n");
    }
}

