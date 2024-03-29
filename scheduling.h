#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_THREADS 256

// Routines
#define N_ROUTINES 5
enum{
    HELP,   
    CLOSE,  
    READ,   
    STORE,  
    SEND,   
};
static const char* ROUTINES[] = {"help", "close", "read", "store", "send"};


// Actions
enum{
    START,
    END,
    ERROR
};
static const char* ACTIONS[] = {"start", "end", "error"};

// return values
enum{
    NONE = -2, 
    WRONG = -3
};


// Structs
struct th_info {
    // Id to identify the thread
    unsigned long id;
    // Period of the task
    int period;
    // Time needed by the task to complete
    int comptime;
    // Priority of the thread (0 - 100)
    int priority;
    // Defines which taks this thread represents
    int type; 
};

struct thread {
    struct th_info info;    // Thread info
    pthread_t *__restrict thread;
};

/*
    Function to check if a set of task is schedulable using the boundary ln(2) and 1.

    @return true if the set of task is schedulable for sure
*/
int is_RM(struct thread*, int);

/*
    Function to check the schedulability of the set of tasks

    @param struct thread* array of thread(s)
    @param int number of threads
    @return 1 if it's schedulable, 0 otherwise
*/
int is_schedulable(struct thread*, int);

/*
    Function to order the threads in descending order (1 - higher, 100 - lower) with respect to their priority

    @param struct thread* array of thread(s)
    @param int number of threads
*/
void order_ths(struct thread*, int);

#endif