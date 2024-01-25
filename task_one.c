#include <stdio.h>
#include <time.h>

// Schedulable

/*
    Period 5s 
    Computation time 1s 

*/


int main() {
    clock_t start_time_print = clock();
    clock_t end_time, start_time;
    double elapsed_time, elapsed_time_print;
    int c = 0;

    struct timespec time, rem;
    time.tv_sec = 0;
    time.tv_nsec = (long) (395 * 1000000); 
    
    start_time_print = start_time = clock();
        // start_time = clock();
    while(!c /* !nanosleep(&time, NULL) */){
        // 20ms execution
        while (1) {
            end_time = clock();
            elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            
            elapsed_time_print = ((double)(end_time - start_time_print)) / CLOCKS_PER_SEC;

            // if (elapsed_time_print >= 1){
            //     printf("Read: %d\n", c++);
            //     start_time_print = end_time;  
            // }
            
            if (elapsed_time >= 0.1) {
                printf("Read task: %d\n", c++);
                start_time = end_time;  
                break;
            }

        }   
        nanosleep(&time, NULL);
    }

    return 0;
}
