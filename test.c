#include <stdio.h>
#include <math.h>

// int nums[] = {37,11,7,5};
// int numbers = 4, 
int active_threads = 3;

// int threads_test[] = {5,4,3,2,1};

// int lcm(int index, int mcm){
//     if(index == numbers-1) return mcm;

//     int b = nums[++index];

//     int max = (mcm > b) ? mcm : b;
//     while((max % mcm != 0) || (max % b != 0))
//         max++;
    
//     printf("found %d\n", max);

//     return lcm(index, max);
// }

// Structs
struct th_info {
    unsigned long id;
    int period;
    int comptime;
    int priority;
    // float utilization;
};

struct thread {
    struct th_info info;    // Thread info
    int rem_time;           // Remaining time
};

void order_ths(struct thread* threads_test){
    struct thread tmp;

    for(int i=0; i<active_threads-1; i++)
        if(threads_test[i].info.priority > threads_test[i+1].info.priority){
            for(int j=i; j<active_threads-1; j++){
                tmp = threads_test[j];
                threads_test[j] = threads_test[j+1];
                threads_test[j+1] = tmp;
            }
            i--;
        }
}



// struct thread threads[] = {{{3,50,10,3},3},{{2,30,6,2},2},{{1,20,10,1},1}};
struct thread threads[] = {
    {{3, 5, 1, 3}, 3},
    {{2, 3, 1, 2},2},
    {{1, 2, 1, 1},1},
};

int is_RM(){return 0;}

int is_schedulable(){
    if(is_RM()) return 1;   // The task set respects the U_lub

    order_ths(threads);            // Order priorities in descending order
    int expired = 0;        // Flag to check if a deadline is not respected

    int prev_respt, curr_respt;

    // Check all threads
    for(int i=0; !expired && i<active_threads; i++){
        printf("--- Task %d ---\n", i+1);

        curr_respt = threads[i].info.comptime;            // Just to pass the next while
        prev_respt = curr_respt-1;           

        // Continue for (undefined) iterations
        int c = 0;
        while(prev_respt < curr_respt && !expired){
            prev_respt = curr_respt;
            // printf("[iter: %d] New approx: ", i+1);

            // Consider all interferences from higher-priority tasks
            curr_respt = threads[i].info.comptime;
            
            printf("w(%d) = %d", c, curr_respt);
            for(int j=i-1; j>-1; j--){
                printf(" + ceil(%d / %d) * %d", prev_respt, threads[j].info.period, threads[j].info.comptime);
                curr_respt += ((int)ceil((double)prev_respt/(double)threads[j].info.period)) * threads[j].info.comptime;
            }

            printf(" = %d\n", curr_respt);

            if(curr_respt > threads[i].info.period){
                expired = 1;  
                break;
            }

            c++;
            
        }
        if(expired) break;
        
    }

    return !expired;
}

int main(){

    printf("Schedulable: %d\n", is_schedulable());

    // printf("lcm: %d\n" , lcm(0, nums[0]));

    // for(int i=0; i<active_threads; i++) printf("%d ", threads_test[i]);
    // printf("\n");
    // order_ths();
    // for(int i=0; i<active_threads; i++) printf("%d ", threads_test[i]);
    // printf("\n");
    return 0;
}
