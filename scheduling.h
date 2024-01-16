#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_THREADS 256

// Routines
#define N_ROUTINES 5
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
    pthread_t *__restrict thread;
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
int is_RM(struct thread*, int);

/*
    Function to check if the schedulability of the set of tasks

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