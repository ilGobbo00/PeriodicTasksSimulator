#ifndef UTILITIES_H
#define UTILITIES_H

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_THREADS 256

extern int active_threads;

/*
    Recursive function to find mcm

    @param int index fist position of the array
    @param int mcm first value 
*/
int lcm(int, int);

/*
    Function to check if a set of task is schedulable using the boundary ln(2)
    @return true if the set of task is schedulable for sure
*/
int is_RM();

/*
    Function to check if the schedulability of the set of tasks
*/
int is_schedulable();

/*
    Function to initialize and run the server
    @param int port for listen incoming connection
*/
int create_server(int);

/*
    Listen for incoming connection once the server is active. 
    It checks for the schedulability of the tasks and eventually starts new threads
    @param int socket for connection 
*/
int listen(int);

#endif