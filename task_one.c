#include <time.h>

#include <stdio.h>
#include <unistd.h>

// Schedulable
/*
    Period 50ms
    Computation time 20ms (avg on 10 trials)

*/


int main(){
    int computation = 20;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("nanosleep error\n");

    return 0;
}