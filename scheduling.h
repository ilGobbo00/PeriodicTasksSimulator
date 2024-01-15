#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_THREADS 256

// Routines
enum{
    HELP,   
    CLOSE,  
    READ,   
    WRITE,  
    SEND,   
};
static const char* ROUTINES[] = {"help", "close", "read", "write", "send"};

// Actions
enum{
    START,
    END,
    ERROR
};
static const char* ACTIONS[] = {"start", "end", "error"};

// return value
enum{
    NONE, 
    WRONG
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
    pthread_t * thread;
};

/*
    Recursive function to find mcm

    @param int index fist position of the array
    @param int mcm first value 
*/
// int lcm(int, int);

/*
    Function to check if a set of task is schedulable using the boundary ln(2).
    It works also with only one task active.

    @return true if the set of task is schedulable for sure
*/
int is_RM(int, struct thread*);

/*
    Function to check if the schedulability of the set of tasks
*/
int is_schedulable(int, struct thread*);

/*
    Function to order the threads in descending order (1 - higher, 100 - lower) with respect to their priority
*/
void order_ths();

#endif