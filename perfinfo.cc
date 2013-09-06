/* perfinfo.cc
 * collect the perf information
 */

//#include "perfinfo.h"
#include <iostream>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <perfmon/pfmlib_perf_event.h>
#include <err.h>

static int enabled = 0;
static int *perf_event_fds;
static struct perf_event_attr *perf_event_attrs;

int perfeventinit(int numEvents)
{
  int ret = pfm_initialize();
  if (ret != PFM_SUCCESS)
  {
    // all following should be updated to log msg style later
    std::cout << "error in pfm_initialize: " << pfm_strerror(ret) << std::endl;
    return -1;
  }

  perf_event_fds = (int *)calloc(numEvents, sizeof(int));
  if (!perf_event_fds)
  {
    std::cout << "error allocating perf event fds" << std::endl;
    return -1;
  }
  perf_event_attrs = (struct perf_event_attr *)calloc(numEvents, sizeof(struct perf_event_attr));
  if (!perf_event_attrs)
  {
    std::cout << "error allocating perf event attrs" << std::endl;
    return -1;
  }
  for(int i=0; i<numEvents; i++)
    perf_event_attrs[i].size = sizeof(struct perf_event_attr);

  enabled = 1;
  return 1;
}

int perfeventcreate(int id, const char *EventName)
{
  struct perf_event_attr *pe = (perf_event_attrs + id);
  int ret = pfm_get_perf_event_encoding(EventName, PFM_PLM3, pe, NULL, NULL);
  if (ret != PFM_SUCCESS) {
    std::cout << "error creating event" << id << " " << EventName << ": " << pfm_strerror(ret) << std::endl;
    return -1;
  }

  pe->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;
  pe->disabled = 1;
  pe->inherit = 1;
  perf_event_fds[id] = perf_event_open(pe, 0, -1, -1, 0);
  if (perf_event_fds[id] == -1){
    std::cout << "error in perf_event_open for event " << id << ", " << EventName;
    return -1;
  }
}

int perfeventenable()
{
  if (enabled){
    if (prctl(PR_TASK_PERF_EVENTS_ENABLE)) {
      std::cout << "error in prctl(PR_TASK_PERF_EVENTS_ENABLE)" << std::endl;
      return -1;
    }
    return 0;
  }
}

int perfeventdisable()
{
  if (enabled){
    if (prctl(PR_TASK_PERF_EVENTS_DISABLE)) {
      std::cout << "error in prctl(PR_TASK_PERF_EVENTS_DISABLE)" << std::endl;
      return -1;
    }
    return 0;
  }
}

int perfeventread(int id, long long *value)
{
  size_t expectedBytes = 3 * sizeof(long long);
  int ret = read(perf_event_fds[id], value, expectedBytes);
  std::cout << perf_event_fds[id] << " is ret\n";
  if (ret < 0) {
    std::cout << "error in reading event: " << strerror(errno) << std::endl;
    return -1;
  }
  if (ret != expectedBytes) {
    std::cout << "read of perf event did not return 3 64-bit values\n";
    return -1;
  }
  return 0;
}

// following is the test code, comment after test successful 
//#include <pthread>
int main(int argc, char **argv)
{
  if(perfeventinit(3)<0)
  {
    return 0;
  }

  std::cout << "initial passed\n";
  // create the perf events
  // usage can be in array or other portable data structures
  if (perfeventcreate(0, "instructions")<0||perfeventcreate(1, "cycles")<0||perfeventcreate(2, "LLC-stores")<0)
    return 0;

  std::cout << "events created\n";
  // enable the perf event collection
  if (perfeventenable<0)
    return 0;

  std::cout << "events enabled\n";
  // execute the following codes for collection
  long int i = 0;
  unsigned long j = 0;
  for (i=0;i<99999;i++)
    j=i+1;

  std::cout << "execution finished\n";
  //read the information
  long long *perfinfo;
  perfinfo = (long long *)malloc(sizeof(long long));
  if (perfeventread(0, perfinfo)==0)
    std::cout << "instructions: " << *perfinfo << std::endl;
  else
    return 0;

  if (perfeventread(1, perfinfo) == 0)
    std::cout << "cycles: " << *perfinfo << std::endl;
  else
    return 0;

  if (perfeventread(2, perfinfo) == 0)
    std::cout << "LLC-stores: " << *perfinfo << std::endl;
  else
    return 0;

  perfeventdisable;
  return 0;
}
