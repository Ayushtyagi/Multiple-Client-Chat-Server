#include "server_header.h"
#include "logger.h"
void stub_call(NODE);

int main()
{
        NODE arg_temp;
        arg_temp.sd = 898;
        strcpy(arg_temp.username, "ayush");
        stub_call(arg_temp);
    return 0;
}
 
 
void stub_call(NODE arg)
{
        log_level = LOG_LEVEL;
        log_max = LOG_MAX;
        strcpy(log_file,"log.txt");
        strcpy(error_log_file,"error_log.txt");
        pthread_t thread;
        int ret = -1;
        ret = pthread_create(&thread, NULL, thread_server, (void*)&arg);
        if(ret != 0)
        {
                perror("Thread create error\n");
        }
        pthread_join(thread, NULL);
}

