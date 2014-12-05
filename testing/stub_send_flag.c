#include "server_header.h"
#include "logger.h"
//#include<stdlib.h>
void stub_call(int , int );


int main()
{
    int flag = 6;
    stub_call(9998,6);
    stub_call(675,flag);
    //stub_call(12,flag);
    //stub_call(24,NULL);
    return 0;
}





void stub_call(int sd, int flag)
{
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");	
	
       send_flag(sd,flag);

    if((0 == flag) || (1 == flag))
    {
	printf("\n update successful\n");	
    }
    else
    {
	printf("\n not functioning\n");
    }
}

