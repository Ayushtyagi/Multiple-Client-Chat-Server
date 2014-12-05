#include"server_header.h"
#include"logger.h"

void stub_call(int,char*);

int main()
{
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");

	
	stub_call(LOG_CRITICAL,"error in file open");
    
	return 0;
}

void stub_call(int log_level,char* message)
{
	LOGGER(log_level, message);
}

