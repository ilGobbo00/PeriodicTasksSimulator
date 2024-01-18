#include "scheduling.h"

int is_RM(struct thread* thrs, int n_threads){
    double tot_utilization = 0;
    
    // Get the sum of all utilizations
    for(int i=0; i<n_threads; i++)
        tot_utilization += (double)thrs[i].info.comptime / (double)thrs[i].info.period;

    printf("Tot utilization %f in %d threads\n", tot_utilization, n_threads);

    // Compare with the threshold
    if(n_threads > 10)
        return tot_utilization < log(2);
    else
        return tot_utilization < (double)n_threads * (pow(2, (double)1/(double)n_threads) - 1);
}

void order_ths(struct thread* thrs, int n_threads){
    struct thread tmp;

    for(int i=0; i<n_threads-1; i++)
        if(thrs[i].info.priority > thrs[i+1].info.priority){
            for(int j=i; j<n_threads-1; j++){
                tmp = thrs[j];
                thrs[j] = thrs[j+1];
                thrs[j+1] = tmp;
            }
            i--;
        }
}

int is_schedulable(struct thread* thrs, int n_threads){
    if(is_RM(thrs, n_threads)) return 1;   // The task set respects the U_lub

    order_ths(thrs, n_threads);            // Order priorities in descending order
    int expired = 0;        // Flag to check if a deadline is not respected

    int prev_respt, curr_respt;
    
    // Arrays to store the response
    char resp[1000];
        bzero(resp, 1000);

    // Check all threads
    for(int i=0; !expired && i<n_threads; i++){
        sprintf(resp + strlen(resp), "\n--- Task %d ---", i+1);

        // Initialize variables
        curr_respt = thrs[i].info.comptime;            
        prev_respt = curr_respt-1;                          // Just to pass the next while

        

        // Continue for (undefined) iterations
        int c = 0;                                       // Number of cycles (for debugging purposes)
        while(prev_respt < curr_respt && !expired){
            // Save the previous response time
            prev_respt = curr_respt;

            // Get the thread's computational time 
            curr_respt = thrs[i].info.comptime;
            
            // Consider all interferences from higher-priority tasks
            sprintf(resp + strlen(resp), "\nw(%d) = %d", c, curr_respt);
            for(int j=i-1; j>-1; j--){
                sprintf(resp + strlen(resp), " + ceil(%d / %d) * %d", prev_respt, thrs[j].info.period, thrs[j].info.comptime);
                curr_respt += ((int)ceil((double)prev_respt/(double)thrs[j].info.period)) * thrs[j].info.comptime;
            }

            sprintf(resp + strlen(resp), " = %d", curr_respt);

            // Check for theadlines
            if(curr_respt > thrs[i].info.period){
                printf("%s > %d\n\n", resp, thrs[i].info.period);
                return 0;                               // Not schedulable 
                break;
            }

            c++;
        }
    }

    return 1;       // Schedulable 
}
