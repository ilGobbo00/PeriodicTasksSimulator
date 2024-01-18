#include <time.h>

#include <stdio.h>
#include <unistd.h>

// Schedulable
/*
    Period 16ms
    Computation time 2 (avg on 10 trials)

*/


int main(){
    clock_t start_time_print = clock();
    clock_t end_time, start_time;
    double elapsed_time, elapsed_time_print;
    int c = 0;

    struct timespec time, rem;
    time.tv_sec = 0;
    time.tv_nsec = (long) (995 * 1000000); 
    
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