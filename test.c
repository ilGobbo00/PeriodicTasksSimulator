#include <stdio.h>
#include <math.h>

int nums[] = {37,11,7,5};
int numbers = 4, active_threads = 5;

int threads_test[] = {5,4,3,2,1};

int lcm(int index, int mcm){
    if(index == numbers-1) return mcm;

    int b = nums[++index];

    int max = (mcm > b) ? mcm : b;
    while((max % mcm != 0) || (max % b != 0))
        max++;
    
    printf("found %d\n", max);

    return lcm(index, max);
}

void order_ths(){
    int tmp;

    for(int i=0; i<active_threads-1; i++)
        if(threads_test[i] > threads_test[i+1]){
            for(int j=i; j<active_threads-1; j++){
                tmp = threads_test[j];
                threads_test[j] = threads_test[j+1];
                threads_test[j+1] = tmp;
            }
            i--;
        }
}

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

struct thread threads[] = {{{2,22,5,3},2},{{1,14,4,2},1},{{0,8,3,1},0}};

int is_RM(){return 0;}

int is_schedulable(){
    if(is_RM()) return 1;   // The task set respects the U_lub

    order_ths();            // Order priorities in descending order
    int expired = 0;        // Flag to check if a deadline is not respected

    int prev_respt, curr_respt;

    // Check all threads
    for(int i=0; !expired && i<active_threads; i++){
        prev_respt = threads[i].info.comptime;           
        curr_respt = prev_respt + 1;            // Just to pass the next while

        // Continue for (undefined) iterations
        while(prev_respt < curr_respt && !expired){

            // Consider all interferences from higher-priority tasks
            curr_respt = threads[i].info.comptime;
            for(int j=i-1; j>0; j--){
                curr_respt += ceil(prev_respt/threads[j].info.period) * threads[j].info.comptime;
            }

            if(curr_respt > threads[i].info.period){
                expired = 1;  
                break;
            }
            
            prev_respt = curr_respt;
        }
        
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
