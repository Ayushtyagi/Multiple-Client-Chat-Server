#include"server_header.h"
#include"logger.h"

void stub_call(int);


int main()
{
	log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");
 
	stub_call(2);
	return 0;
}





void stub_call(int signal)
{
       handle_signal(signal);

}

