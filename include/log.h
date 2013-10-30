/* log.h
 * log file for the execution and other records
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>

// open the log file and check whether it exists
FILE *log_open(void);

// record the time, this is used for better read the log msg
int log_time(time_t rawtime, FILE *logfile);

// record the msg to the log file
// need to double-think before final version, whether this is right to write the string not strut
int log_msg(std::string msg, FILE *logfile);

#endif
