/* perfinfo.h
 * collect the perf infomation
 */

/*
#ifdef _PERFINFO_H
#define _PERFINFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// libpfm library, TBD whether need papi.h
#include <perfmon/pfmlib_perf_event.h>
#include <err.h>

static int enabled = 0;
static int *perf_event_fds;
static struct perf_event_attr *perf_event_attrs;

// initial the perf events
int perfeventinit(int numEvents);

// create the perf events
int perfeventcreate(int id, const char *EventName);

// enable the perfevent collection
int perfeventenable(void);

// disable the perfevent collection
int perfeventdisable(void);

// read the perf event information
int perfeventread(int id, long long *value);

#endif

*/
