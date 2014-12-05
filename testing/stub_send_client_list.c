#include"server_header.h"
#include"logger.h"

void stub_call(NODE*, int);

int main()
{
     	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

	NODE* head=NULL;
	int ab=78;
	
	stub_call(head,5);
    	stub_call(NULL,7);
    	stub_call(head,ab);
    
	return 0;
}


void stub_call(NODE* head, int num)
{
	send_client_list(head,num);

	if(head==NULL)
	printf("\nsending client list unsuccessful\n");
	else
	printf("\n sending client list successful\n");

}

