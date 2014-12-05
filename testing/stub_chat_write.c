#include "client_header.h"
#include "logger.h"
#include<stdlib.h>

void stub_call(CNODE);

int main()
{
    int flag = 6;
    CNODE arg;
    arg.sd = 998;
    strcpy(arg.username,"rahul");
    arg.g_number = 2;

    stub_call(arg);
    stub_call(arg);
    stub_call(arg);
    
    return 0;
}

void stub_call(CNODE arg)
{
    log_level = LOG_LEVEL;
    log_max = LOG_MAX;
    strcpy(log_file,"log.txt");
    strcpy(error_log_file,"error_log.txt");	
	
    chat_write(&arg);

}

