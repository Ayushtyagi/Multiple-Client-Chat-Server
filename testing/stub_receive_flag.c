#include "server_header.h"
#include "logger.h"
//#include<stdlib.h>
void stub_call(int , int *);


int main()
{
    int flag = 6;
    stub_call(999,&flag);
    //stub_call(12,flag);
    //stub_call(24,NULL);
    return 0;
}





void stub_call(int sd, int* flag)
{
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");	
	
	int lflag;
	lflag=*flag;
       receive_flag(sd,&lflag);

    if((0 == lflag) || (1 == lflag))
    {
	printf("\n update successful\n");	
    }
    else
    {
	printf("\nnot functioning\n");
    }
}

