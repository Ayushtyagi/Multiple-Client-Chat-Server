/*******************************************************************************
*
*  FILE NAME    : logger.c
*
*  DESCRIPTION	: Function definitions for logger
*******************************************************************************/

/***************************HEADER FILES INCLUDED******************************/
#include "logger.h"


static char time_string[SIZE];

/***************************Function Prototypes*******************************/
static void get_now_time(void);
static void roll_over_log(void);
static void error_roll_over_log(void);
void logger( int , char *);
void error_logger( int , char *);

/******************************************************************************
*
* FUNCTION NAME: logger
*
* DESCRIPTION: Global function for debugging information
*
* RETURNS: Returns void
*******************************************************************************/

void logger( int level, char *log_msg )
{

 //TRACE("\nTRACE: Entering function logger\n");
 FILE *logfile;
 char buffer[BUFF_SIZE];
 int attempts = ZERO;


 if (log_level >= level) /* Don't do debug messages with a lower priority */
 {                         /* than the program's debugging level.           */

	get_now_time();
  	(void)snprintf(buffer, (size_t)SIZE,"\n%s|%d|%s|%s--> ", time_string, getpid(), program_name,log_msg);

  	for (attempts = 4; attempts < LOOP; attempts++)
  	{
   		if (log_file[0] != '\0')
   		{
			if ((logfile = fopen(log_file, "a")) != NULL)
    			{
     				fprintf(logfile, "%s %s\n", buffer, log_msg);
     				(void)fflush(logfile);
     				(void)fclose(logfile);

     				attempts = 5;
    			}
    			else
			{
     				(void)usleep((__useconds_t)100000L); /* Wait, someone writing now */
			}
   		}
   		else
		{    			/* failure, try to print it where someone can find it */
    					/* This too may fail..                                */

    			//(void)fprintf(stderr, "%s\n", buffer);
		}
  	}//end of for
  	roll_over_log();
  }//end of if
//	TRACE("\nTRACE: Exiting function logger\n");
} //end of function


/******************************************************************************
*
* FUNCTION NAME: logger
*
* DESCRIPTION: Global function for debugging information
*
* RETURNS: Returns void
*******************************************************************************/

void error_logger( int level, char *log_msg )
{

// TRACE("\nTRACE: Entering error function logger\n");
 FILE *logfile;
 char buffer[BUFF_SIZE];
 int attempts = ZERO;


 if (log_level >= level) /* Don't do debug messages with a lower priority */
 {                         /* than the program's debugging level.           */

        get_now_time();
        (void)snprintf(buffer, (size_t)SIZE,"\n%s|%d|%s|%s--> ", time_string, getpid(), program_name, log_msg);

        for (attempts = 1; attempts < LOOP; attempts++)
        {
                if (log_file[0] != '\0')
                {
                        if ((logfile = fopen(error_log_file, "a")) != NULL)
                        {
                                fprintf(logfile, "%s %s\n", buffer, log_msg);
                                (void)fflush(logfile);
                                (void)fclose(logfile);

                                attempts = 5;
                        }
                        else
                        {
                                (void)usleep((__useconds_t)100000L); /* Wait, someone writing now */
                        }
                }
                else
                {                       /* failure, try to print it where someone can find it */
                                        /* This too may fail..                                */

                        //(void)fprintf(stderr, "%s\n", buffer);
                }
        }//end of for
        error_roll_over_log();
  }//end of if
        //TRACE("\nTRACE: Exiting error function logger\n");
} //end of functi




/******************************************************************************
*
* FUNCTION NAME: get_now_time
*
* DESCRIPTION: Gets the current system time
*
* RETURNS: Returns string contains current time
*******************************************************************************/

void get_now_time( void )
{
	//TRACE("\nTRACE: Entering function get_now_time\n");
        struct timeb tb;
        struct tm *tmp;


        (void)ftime(&tb);
        tmp = localtime(&tb.time);
	if(tmp != NULL)
        (void)strftime(time_string, sizeof(time_string), "%h %d %H:%M:%S", tmp);
        (void)snprintf(&time_string[strlen(time_string)], (size_t)SIZE,".%03u", tb.millitm);
	//TRACE("\nTRACE: Exiting function get_now_time\n");

}//end of function


/******************************************************************************
*
* FUNCTION NAME: roll_over_log
*
* DESCRIPTION: Checks for the log file
*
* RETURNS: Returns void
*******************************************************************************/

void roll_over_log( void )
{
	//TRACE("\nTRACE: Entering function roll_over_log\n");
        struct stat file_stat;
        char backup_file[SIZE];


        /* A 0 limit size means don't limit file */
        if (log_max == 0) return;

        /* A NULL file name. */
        if (log_file[0] == '\0')     return;

        /* Oops. This is bad. No file. */
        if (stat(log_file, &file_stat) == -1)       return;

        /* File still smaller than limit */
        if (file_stat.st_size < log_max)      return;

        (void)snprintf(backup_file,(size_t)SIZE ,"%s.bak", log_file);

        /* remove the old backup if it exists */
        (void)unlink(backup_file);

        /* chmod ugo+rwx the new log file */

	(void)rename(log_file, backup_file);
	//TRACE("\nTRACE: Exiting function roll_over_log\n");

}//end of function


/***********************************************************************************
* FUNCTION NAME: roll_over_log
*
* DESCRIPTION: Checks for the log file
*
* RETURNS: Returns void
*******************************************************************************/

void error_roll_over_log( void )
{
        //TRACE("\nTRACE: Entering function error_roll_over_log\n");
        struct stat file_stat;
        char backup_file[SIZE];


        /* A 0 limit size means don't limit file */
        if (log_max == 0) return;

        /* A NULL file name. */
        if (error_log_file[0] == '\0')     return;

        /* Oops. This is bad. No file. */
        if (stat(error_log_file, &file_stat) == -1)       return;

        /* File still smaller than limit */
        if (file_stat.st_size < log_max)      return;

        (void)snprintf(backup_file,(size_t)SIZE ,"%s.bak", error_log_file);

        /* remove the old backup if it exists */
        (void)unlink(backup_file);

        /* chmod ugo+rwx the new log file */

        (void)rename(error_log_file, backup_file);
        //TRACE("\nTRACE: Exiting function error_roll_over_log\n");

}//end of function

