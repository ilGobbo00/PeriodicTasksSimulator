/*

    
    It checks for the schedulability of the tasks and eventually starts new threads
*/

#include "network.h"
#include "stdio.h"
#include <signal.h>
#define DEBUG printf("------- HERE -------\n");

// Routines 
/*
    HIGH PRIORITY TASK
    PERIOD: 2s
    COMPUTATIONAL TIME: 1s

    Average Period over 10 executions: 2.003 seconds
    Average CPU Time over 10 executions: 1.001 seconds
*/
void read_datac();

/*
    MEDIUM PRIORITY TASK
    PERIOD: 3s
    COMPUTATIONAL TIME: 1s

    Average Period over 10 executions: 2.008 seconds
    Average CPU Time over 10 executions: 1.001 seconds
*/
void store_datac();

/*
    LOW PRIORITY TASK
    PERIOD: 5s
    COMPUTATIONAL TIME: 1s

    Average Period over 10 executions: 5.022 seconds
    Average CPU Time over 10 executions: 1.001 seconds
*/
void send_datac();

// Utilities

/*
    Checks whether the id is already used by an existing thread

    @param struct thread - the list of threads
    @param int - number of threads
    @param int - id to search for
    @return 1 if it exists, 0 otherwise
*/ 
int existing_id(struct thread ths[], unsigned int active_th, int id);

int start_thread(int s, struct thread* threads, struct thread* th_analysis, unsigned int* active_threads, int id, int period, int comptime, int priority, int type, void* function);
int close_thread(int s, struct thread* threads, unsigned int* active_threads, int id);   


int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage:\n\t%s <port between 1024 and 65535>\n", argv[0]);
        return 1;
    }

    int port, s_socket, c_socket;
    sscanf(argv[1], "%d", &port);

    // TODO dopo aver catturato il EOF con il read, togliere togliere il comando per ignorare il segnale
    // signal(SIGPIPE, SIG_IGN);

    char help[] = 
        "   <task> <'start'/'end'> <id bigger than 5>\n\n"
        "           available task: read\n"
        "                           store\n"
        "                           send\n";

    // threads' info
    int period, comptime, priority, type;
    void* r_to_exe; 
    
    // Serve a client at time
    if((s_socket = create_server(port)) == -1){
        printf("[x] Error while creating server\n");
        return 1;
    }
    while(1){



        printf("[.] Listening for incoming connections..\n");
        if((c_socket = conn_listen(s_socket, port)) == -1){
            printf("[x] Error while listening for incoming connections\n");
            continue;
        } 

        unsigned int id = 0, active_threads = 0;
        int routine, action, ret_id;
        int close_client = 0;
        struct thread threads[MAX_THREADS];
        struct thread th_analysis[MAX_THREADS];

        bzero(threads, MAX_THREADS * sizeof(struct thread));
        bzero(th_analysis, MAX_THREADS * sizeof(struct thread));

    
        while(!close_client){
            // Print existing routines
            printf("[i] Active routines' ids (%d): ", active_threads);
            for(int k=0; k<active_threads; k++)
                printf("%ld ", threads[k].info.id);
            printf("\n");

            close_client = 0;
            routine = action = -1;
            ret_id = listen_for_commands(c_socket, &routine, &action);

            // BUG  se il ret_id == .. allora la connessione é stata chiusa
            // read error
            if(ret_id == -1){
                printf("[x] Command listen error\n");
                continue;
            }
            // wrong client input
            if(ret_id == WRONG){
                printf("[-] Invalid command\n");
                send_data(c_socket, "Invalid command");
                continue;
            }

            // no errors, correct user input
            
            // if a new thread will be started, prepare to analyse
            if(routine > CLOSE && action == START){
                // Check if maximum number of threads was reached
                if(active_threads+1 > MAX_THREADS){
                    printf("[i] Max number of thread reached\n");
                    send_data(c_socket, "Function not started - max number of threads reached");
                    continue;
                }
                
                // Check if the id is available
                if(existing_id(threads, active_threads, ret_id)){
                    send_data(c_socket, "Function not started - id already used: Use a different id");
                    continue;
                }
            }

            // Check if a routine has to be shutted
            if(routine > CLOSE && action == END){
                // Check if the id exists
                if(!existing_id(threads, active_threads, ret_id)){
                    printf("[-] Thread not existing\n");
                    send_data(c_socket, "No action performed - id does not exist");
                    continue;
                }
                // If it does, close thread
                if(!close_thread(c_socket, threads, &active_threads, ret_id)){
                    printf("[+] Thread id: %d closed correctly\n", ret_id);
                    send_data(c_socket, "Task ended");
                }else{
                    printf("[!] Some error while closing the thread id: %d\n", ret_id);
                    send_data(c_socket, "Some error occured while closing the task");
                }
                continue;
            }
            
            // Check for errors in action
            if(routine > CLOSE && action == ERROR){
                printf("[!] Action is not correct considering the selected routine\n");
                send_data(c_socket, "No action performed - action is not correct");
                continue;
            }
            // If there is enough room and the id is available, preapre for the analysis
            if(routine > CLOSE) memcpy(th_analysis, threads, MAX_THREADS * sizeof(struct thread));
            
            
            

            // routine and actions are correct (no further checks need to be done)
            switch(routine){
                case CLOSE: 
                    send_data(c_socket, "bye");
                    close_client = 1;
                    continue;
                break;

                case HELP: 
                    send_data(c_socket, help);
                    continue;
                break;

                case READ: 
                            period = 2;
                            comptime = 1;
                            priority = 1;
                            type = READ;
                            r_to_exe = (void*)read_datac;
                break;

                case STORE: 
                            period = 3;
                            comptime = 1;
                            priority = 2;
                            type = STORE;
                            r_to_exe = (void*)store_datac;
                break;

                case SEND: 
                            period = 5;
                            comptime = 1;
                            priority = 3;
                            type = SEND;
                            r_to_exe = (void*)send_datac;
                break;

                default:
                    printf("[-] Invalid routine\n");
                    send_data(c_socket, "Invalid routine");
                    continue;
            }

            int res = start_thread(c_socket, threads, th_analysis, &active_threads, ret_id, period, comptime, priority, type, r_to_exe);
            
            if(!res){
                printf("[+] New thread started\n");
                send_data(c_socket, "New task started!");
            }else{
                if(res == -1){
                    printf("[!] Failed to start the thread\n");
                    send_data(c_socket, "Function not started - internal error");
                }else{
                    printf("[-] Task not schedulable\n");
                    send_data(c_socket, "Function not started - task not schedulable");
                }
            }
        }

        // Kill all remaining zombies
        int res = 0, i;
        for(i=0; i<active_threads; i++)
            res |= pthread_cancel(*threads[i].thread);
        if(res)
            printf("[x] No all remaining threads were succesfully closed\n");
        else if(i>0) printf("[+] All remaining threads were correctly closed\n");
        
        active_threads = 0;
    }

    return 0;
}

int start_thread(int s, struct thread* threads, struct thread* th_analysis, unsigned int* active_threads, int id, int period, int comptime, int priority, int type, void* function){

    if(existing_id(threads, *active_threads, id)){
        printf("[x] Already existing id\n");
        // send_data(s, "ID already existing");
        return -3;
    }

    struct thread new_th = {{id, period, comptime, priority, type}, malloc(sizeof(pthread_t))};
    th_analysis[*active_threads] = new_th;

    if(!is_schedulable(th_analysis, (*active_threads)+1)){
        printf("[x] Non schedulable thread\n");
        // send_data(s, "Function not started - not schedulable thread");
        return -2;
    }

    unsigned int* p_id = malloc(sizeof(unsigned int));
    *p_id = id;

    if(pthread_create(new_th.thread, NULL, function, p_id)){
        printf("[x] Error while creating a new thread\n");
        free(new_th.thread);
        // send_data(s, "Function not started - error while creating a new thread");
        return -1;
    }

    if(!pthread_setschedprio(*new_th.thread, priority))
        printf("[!] Can't set the priority to the new thread\n");

    threads[*active_threads] = new_th;

    (*active_threads)++;

    return 0;
}

int close_thread(int s, struct thread* threads, unsigned int* active_threads, int id){
    int j; 
    for(j=0; j<*active_threads && threads[j].info.id != id; j++);

    if(pthread_cancel(*threads[j].thread)){
        printf("[!] It was not possible to stop the thread\n");
        // send_data(s, "Function not stopped - thread was not stopped due to an error");
        return -1;
    }

    free(threads[j].thread);

    // Compat other active threads
    for(; j<*active_threads-1; j++)
        threads[j] = threads[j+1];
    
    (*active_threads)--;

    printf("[+] Thread correctly stopped\n");
    // send_data(s, "Function correclty stopped");

    return 0;
}

int existing_id(struct thread ths[], unsigned int active_th, int id){
    for(int i=0; i<active_th; i++)
        if(ths[i].info.id == id)
            return 1;
    return 0;
}

void read_datac(void* arg){
    unsigned int* id = (unsigned int*) arg;
    clock_t start_time_print = clock();
    clock_t end_time, start_time;
    double elapsed_time, elapsed_time_print;
    int c = 0;

    struct timespec time, rem;
    time.tv_sec = 0;
    time.tv_nsec = (long) (995 * 1000000); 
    
    while(!nanosleep(&time, NULL)){
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
                printf("Read task %d: %d\n", *id, c++);
                start_time = end_time;  
                break;
            }

        }   
        // nanosleep(&time, NULL);
    }
    free(id);
}

void store_datac(void* arg){
    unsigned int* id = (unsigned int*) arg;

    clock_t start_time_print = clock();
    clock_t end_time, start_time;
    double elapsed_time, elapsed_time_print;
    int c = 0;

    struct timespec time, rem;
    time.tv_sec = 1.7;
    time.tv_nsec = (long) (0 * 1000000); 
    
    while(!nanosleep(&time, NULL)){
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
                printf("Store task %d: %d\n", *id, c++);
                start_time = end_time;  
                break;
            }
        }   
        // nanosleep(&time, NULL);
    }
    free(id);
}

void send_datac(void* arg){
    unsigned int* id = (unsigned int*) arg;
    clock_t start_time_print = clock();
    clock_t end_time, start_time;
    double elapsed_time, elapsed_time_print;
    struct timespec time, rem;
    int c = 0;

    while(1){
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
                printf("Send task %d: %d\n", *id, c++);
                start_time = end_time;  
                break;
            }

        }
        time.tv_sec = 4;
        time.tv_nsec = (long) (0 * 1000000); 
        nanosleep(&time, NULL);
    }
    free(id);
}

