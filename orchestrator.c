#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "utilities.h"

#define MAX_THREADS 256

// Structs
struct th_info {
    unsigned long id;
    int period;
    int exetime;
    int priority;
    // float utilization;
};

struct thread {
    struct th_info info;     // Thread info
    pthread_t * thread;
    int check_period;   // Count to check the execution within each period
    int rem_time;       // Remaining time
}threads[MAX_THREADS];

// Global vars
int active_threads;

// Routines 
void read_data();
void store_data();
void send_data();


int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage:\n\t%s <port>", argv[0]);
        return 1;
    }

    int port, socket;
    sscanf(argv[1], "%d", &port);

    if((socket = create_server(port)) == -1){
        printf("Error while creating server\n");
        return 1;
    }

    if(listen(socket) == -1){
        printf("Error while listening for incoming connections\n");
        return 1;
    } 

    return 0;
}


void read_data(){
    int computation = 2;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: read_data\n");
}

void store_data(){
    int computation = 4;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: store_data\n");
}

void send_data(){
    int computation = 20;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: send_data\n");
}