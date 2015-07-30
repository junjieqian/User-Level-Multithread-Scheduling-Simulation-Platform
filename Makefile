CC = g++
DEBUG = -g
OBJS = log.o perfinfo.o algorithm.o cmdline.o
CFLAGS = -c -Wall $(DEBUG) -I./include
LDFLAGS = -pthread

scheduler:$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o scheduler

log.o:log.cc
	$(CC) $(CFLAGS) ./src/log.cc

perfinfo.o:perfinfo.cc
	$(CC) $(CFLAGS) -lpfm ./src/perfinfo.cc

algorithm.o:algorithm.cc
	$(CC) $(CFLAGS) ./src/algorithm.cc

cmdline.o:cmdline.cc
	$(CC) $(CFLAGS) cmdline.cc

.PHONY:clean

clean:
	rm -f *.o scheduler
