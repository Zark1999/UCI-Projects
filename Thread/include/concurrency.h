#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <curses.h>
#include <pthread.h>

#include <semaphore.h>

#include "stats.h"
#include "wrapper_functions.h"

#define HISTSIZE 10

#define SEQUENTIAL      0
#define CONCURRENT_FG   1
#define CONCURRENT_CG   2
#define READERWRITER    3

#define READSLEEP 100 // microseconds

extern int histogram[HISTSIZE];

extern int statpipe[2];
extern int histpipe[2];
extern int cmdpipe[2];


void snapshot_histogram();
void snapshot_stats();

void sequential(char* dirname);
void concurrent_cg(char* dirname);
void concurrent_fg(char* dirname);
void readerwriter(char* dirname);
//
//void* cg_thread(void* arg);
//void* fg_thread(void* arg);

typedef struct {
    int n;
    int mode;
    float mean;
    float median;
} statsnap;

typedef struct {
    int n;
    int hist[HISTSIZE];
} histsnap;

#endif
