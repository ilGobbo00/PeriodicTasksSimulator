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

    while(c < 1){
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

        struct timespec time, rem;
        time.tv_sec = 4;
        time.tv_nsec = (long) (0 * 1000000); 
        nanosleep(&time, NULL);
    }

    return 0;
}
