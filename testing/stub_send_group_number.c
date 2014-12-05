#include "client_header.h"
#include "logger.h"

void stub_call(int , int );


int main()
{
    int group_no = 5;
    stub_call(998,6);
    stub_call(998,group_no);
    //stub_call(12,group_no);
    //stub_call(24,NULL);
    return 0;
}





void stub_call(int sd, int group_no)
{
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");	
		
       send_group_number(sd,group_no);

    if((sd)&&(group_no))
    {
	printf("\n update successful\n");	
    }
    else
    {
	printf("\n not functioning\n");
    }
}

