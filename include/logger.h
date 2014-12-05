/******************************************************************************

**	NAME : logger.h

**	DESCRIPTION : it contains the included files, prototypes of functions 
		      used and macros used throughout
******************************************************************************/


#ifndef _LOGGER_H_
#define _LOGGER_H_

/************************* INCLUDED FILES ********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/**************************** MACROS *********************/
#define DEBUG

#ifdef DEBUG
#define TRACE printf
#else
#define dummy(x)  
#define TRACE dummy
#endif

#define LOG

#ifdef LOG
#define LOGGER logger
#else
#define dummy_log(x,y)  
#define LOGGER dummy_log
#endif


#define ERROR_LOG

#ifdef ERROR_LOG
#define ERROR_LOGGER error_logger
#else
#define error_dummy_log(x,y)
#define ERROR_LOGGER error_dummy_log
#endif


#define SIZE 100
#define BUFF_SIZE 512
#define LOOP 5
#define FILE_SIZE 256

#define LOG_CRITICAL 0
#define LOG_MAJOR 1
#define LOG_MINOR 2
#define LOG_TRIVIAL 3
#define LOG_LEVEL 4
#define LOG_MAX 1000L

#define ZERO 0
#define ONE 1
#define SUCCESS 0

int log_level;
long log_max;
char log_file[FILE_SIZE];
char error_log_file[FILE_SIZE];
char program_name[FILE_SIZE];

void loop_function(void);
void logger( int , char *);
void error_logger( int , char *);

#endif

