/* perfinfo.cc
 * collect the perf information
 */

#include "perfinfo.h"

int perfeventinit(int numEvents)
{
  int ret = pfm_initialize();
  if (re != PFM_SUCCESS)
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
  if (!perf_event_attr)
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
  int ret = pfm_get_perf_event_encoding(eventName, PFM_PLM3, pe, NULL, NULL);
  if (ret != PFM_SUCCESS) {
    std::cout << "error creating event" << id << " " << EventName << ": " << pfm_strerror(ret) << std::endl;
    return -1;
  }

  pe->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;
  pe->disable = 1;
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
  int ret = read(perf_event_fds[id], values, expectedBytes);
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

