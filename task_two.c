#include <time.h>
#include <math.h>

#define MILLIS 1

// Schedulable
/*
    Period 50ms
    Computation time 4

*/

int main(){
    struct timespec time, rem;
    time.tv_sec = 0;
    time.tv_nsec = (long) (MILLIS * 1000000); 
    
    nanosleep(&time, NULL);
    ceil(1.2);

    return 0;
}