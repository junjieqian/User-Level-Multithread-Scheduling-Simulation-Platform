/* log.c
 *
 */

#include "log.h"

FILE *log_open()
{
  FILE *logfile;
  logfile = fopen("simulator-log", "a");

  if(logfile == NULL)
  {
    perror("log file error\n");
    exit(EXIT_FAILURE);
  }

  setvbuf(logfile, NULL, _IOLBF, 0);
  return logfile;
}

int log_msg(std::string msg, FILE *logfile)
{
  if (logfile == NULL)
  {
    perror("log file not exist.\n");
    return -1;
  }

  fprintf(logfile, "%s", msg.c_str());
  return 1;
}

int log_time(time_t rawtime, FILE *logfile)
{
  if (logfile == NULL)
  {
    perror("log file not exist.\n");
    return -1;
  }
  
  struct tm * timeinfo;
//  time(&rawtime);
  timeinfo = localtime(&rawtime);
  
//  const char * c = str.c_str();
  fprintf(logfile, "-------------- TimeStamp: %s", asctime(timeinfo));
  return 1;
}

// following is the test code, comment after test successful
// Test Passed. 08/22/2013
/*
int main(int argc, char **argv)
{
  FILE *log = log_open();
  time_t curtime;
  time(&curtime);
  if (log_time(curtime, log)<0)
  {
    std::cout<<"file written error.\n";
    return 0;
  }

  std::string msg;
  msg = "this is first test\n";
  if (log_msg(msg, log)<0)
  {
    std::cout<< "file written error.\n";
    return 0;
  }

  time(&curtime);
  log_time(curtime, log);
  msg = "this is second test\n";
  log_msg(msg, log);
  return 1;
}
*/

