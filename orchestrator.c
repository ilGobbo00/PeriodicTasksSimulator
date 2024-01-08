#include <stdio.h>
#include <time.h>
#include <pthread.h>

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

// Function declarations

/*
    Recursive function to find mcm

    @param index : fist position of the array
    @param mcm : first value 
*/
int lcm(int, int);

int main(){
    return 0;
}

/*
    Recursive function to find mcm

    @param index : fist position of the array
    @param mcm : first value 
*/
int lcm(int index, int mcm){
    if(index == active_threads-1) return mcm;

    int b = threads[++index].info.period;

    int max = (mcm > b) ? mcm : b;
    while((max % mcm != 0) || (max % b != 0))
        max++;
    
    printf("found %d\n", max);

    return lcm(index, max);
}
/* int lcm(int index, int mcm, int b){
    if(active_threads == 1) return threads[0].info.period;
    if(index == active_threads-1) return mcm;

    int max = (mcm > b) ? mcm : b;
    while((max % mcm != 0) || (max % b != 0))
        max++;
    
    index++;
    return lcm(index, max, threads[index].info.period);
} */



/* int mcm(){
    // Get the minimum even value
    int min = threads[0].info.period;
    unsigned int mcm = 1;
    for(int i=0; i<active_threads; i++){
        if(threads[i].info.period % 2 == 0 && threads[i].info.period < min) 
            min = threads[i].info.period;
        
        // Just to have a value
        mcm *= threads[i].info.period;
    }

    // Find mcm
    short found = 1;
    for(int i=1; !found; i*=min){
        found = 0;
        for(int j=0; j<active_threads; j++){
            found |= threads[j].info.period % min;
        }
    }
} */