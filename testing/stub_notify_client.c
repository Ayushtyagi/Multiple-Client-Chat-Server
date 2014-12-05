#include"server_header.h"
#include"logger.h"

void stub_call(NODE*,int,char*);

int main()
{
	 log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

	NODE* head = NULL;
	int ab=98;
	
	stub_call(head,5,"abc");
	stub_call(head,ab,"123");
	stub_call(head,"ayush","456abc");
	stub_call(head,"nikhil","stupid");
    
	return 0;
}





void stub_call(NODE* head,int fd ,char* buffer)
{
      notify_clients(head,fd,buffer);


    if(head==NULL)
    {
	printf("\n notification failed\n");	
    }
    else
    {
	printf("\n notification successful \n");
    }
}

