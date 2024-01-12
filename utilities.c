#include "utilities.h"


// Structs
struct th_info {
    unsigned long id;
    int period;
    int comptime;
    int priority;
};

struct thread {
    struct th_info info;    // Thread info
    pthread_t * thread;
}threads[MAX_THREADS];

int is_RM(){
    float tot_utilization;
    
    // Get the sum of all utilizations
    for(int i=0; i<active_threads; i++)
        tot_utilization += threads[i].info.comptime / threads[i].info.period;

    // Compare with the threshold
    if(active_threads > 10)
        return tot_utilization < log(2);
    else
        return tot_utilization < active_threads * (pow(2, 1/active_threads) - 1);
}

void order_ths(){
    struct thread tmp;

    for(int i=0; i<active_threads-1; i++)
        if(threads[i].info.priority > threads[i+1].info.priority){
            for(int j=i; j<active_threads-1; j++){
                tmp = threads[j];
                threads[j] = threads[j+1];
                threads[j+1] = tmp;
            }
            i--;
        }
}

int is_schedulable(int n_threads, struct thread* thrs){
    if(is_RM()) return 1;   // The task set respects the U_lub

    order_ths();            // Order priorities in descending order
    int expired = 0;        // Flag to check if a deadline is not respected

    int prev_respt, curr_respt;

    // Check all threads
    for(int i=0; !expired && i<n_threads; i++){
        // printf("--- Task %d ---\n", i+1);

        // Initialize variables
        curr_respt = thrs[i].info.comptime;            
        prev_respt = curr_respt-1;                          // Just to pass the next while

        // Continue for (undefined) iterations
        // int c = 0;                                       // Number of cycles (for debugging purposes)
        while(prev_respt < curr_respt && !expired){
            // Save the previous response time
            prev_respt = curr_respt;

            // Get the thread's computational time 
            curr_respt = thrs[i].info.comptime;
            
            // Consider all interferences from higher-priority tasks
            // printf("w(%d) = %d", c, curr_respt);
            for(int j=i-1; j>-1; j--){
                // printf(" + ceil(%d / %d) * %d", prev_respt, threads[j].info.period, threads[j].info.exetime);
                curr_respt += ((int)ceil((double)prev_respt/(double)thrs[j].info.period)) * thrs[j].info.comptime;
            }

            // printf(" = %d\n", curr_respt);

            // Check for theadlines
            if(curr_respt > thrs[i].info.period){
                return 0;                               // Not schedulable 
                break;
            }

            // c++;
        }
    }

    return 1;       // Schedulable 
}

int create_server(int port){
    // TODO: implement server
    return 0;
}

int listen(int socket){
    // TODO: implement messages receiving
    return 0;
}