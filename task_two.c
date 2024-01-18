#include <time.h>
#include <math.h>
#include <stdio.h>

#define MILLIS 1

// Schedulable
/*
    Period 40ms
    Computation time 40

*/

int main(){
    clock_t start_time_print = clock();
    clock_t end_time, start_time;
    double elapsed_time, elapsed_time_print;
    int c = 0;

    struct timespec time, rem;
    time.tv_sec = 1;
    time.tv_nsec = (long) (700 * 1000000); 
    
    while(c < 1 && !nanosleep(&time, NULL)){
        // 20ms execution
        start_time = clock();
        while (1) {
            end_time = clock();
            elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            elapsed_time_print = ((double)(end_time - start_time_print)) / CLOCKS_PER_SEC;

            // if (elapsed_time_print >= 1){
            //     printf("Data sent : %d\n", c++);
            //     start_time_print = end_time;  
            // }
            
            if (elapsed_time >= 1) {
                printf("Data sent : %d\n", c++);
                start_time = end_time;  
                break;
            }

        }   
        // nanosleep(&time, NULL);
    }

    return 0;
}