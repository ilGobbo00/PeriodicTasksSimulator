/*

    
    It checks for the schedulability of the tasks and eventually starts new threads
*/

#include "network.h"
#include "stdio.h"

// Routines 
void read_datac();
void store_datac();
void send_datac();

// Utilities


// ============== DEBUG
int receive(int sd, char *retBuf, int size)
{
  int totSize, currSize;
  totSize = 0;
  while(totSize < size)
  {
    currSize = recv(sd, &retBuf[totSize], size - totSize, 0);
    if(currSize <= 0)
/* An error occurred */
      return -1;
    totSize += currSize;
  }
  return 0;
}
// =======================

/*
    Checks whether the id is already used by an existing thread

    @param struct thread - the list of threads
    @param int - number of threads
    @param int - id to search for
    @return 1 if it exists, 0 otherwise
*/ 
int existing_id(struct thread ths[], int active_th, int id);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage:\n\t%s <port between 1024 and 65535>\n", argv[0]);
        return 1;
    }

    int port, s_socket, c_socket;
    sscanf(argv[1], "%d", &port);


    char help[] = 
        "   <task> <'start'/'end'> <id>\n\n"
        "   available task: read\n"
        "         store\n"
        "         send\n";

    // threads' info
    int period, comptime, priority, type;
    
    // Serve a client at time
    while(1){

        if((s_socket = create_server(port)) == -1){
            printf("[x] Error while creating server\n");
            return 1;
        }


        printf("[.] Listening for incoming connections..\n");
        if((c_socket = conn_listen(s_socket, port)) == -1){
            printf("[x] Error while listening for incoming connections\n");
            continue;
        } 

        #pragma region DEBUG 
        // DEBUG-START ==============================================
        
        // int 	 currSd;
        // struct   sockaddr_in sin, retSin;
        // /* Create a new socket */
        // if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        // {
        //     perror("socket");
        //     exit(1);
        // }  
        // /* set socket options REUSE ADDRESS */
        //     int reuse = 1;
        //     if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        //         perror("setsockopt(SO_REUSEADDR) failed");
        // /* Initialize the address (struct sokaddr_in) fields */
        // memset(&sin, 0, sizeof(sin));
        // sin.sin_family = AF_INET;
        // sin.sin_addr.s_addr = INADDR_ANY;
        // sin.sin_port = htons(port);

        // /* Bind the socket to the specified port number */
        // if (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1)
        // {
        //     perror("bind");
        //     exit(1);
        // }
        // /* Set the maximum queue length for clients requesting connection to 5 */
        // if (listen(s, 5) == -1)
        // {
        //     perror("listen");
        //     exit(1);
        // }

        // int sAddrLen;
        // sAddrLen = sizeof(retSin);
        // /* Accept and serve all incoming connections in a loop */
        // for(;;)
        // {
        //     if ((currSd =
        //         accept(s, (struct sockaddr *) &retSin, &sAddrLen)) == -1)
        //     {
        //     perror("accept");
        //     exit(1);
        //     }
        //     /* When execution reaches this point a client established the connection.
        //     The returned socket (currSd) is used to communicate with the client */
        //     printf("Connection received from %s\n", inet_ntoa(retSin.sin_addr));

        //     // ---------------------- HANDLE CONNECTION -------------------
        //     unsigned int netLen;
        //     int len;
        //     int exit_status = 0;
        //     char *command, *answer;
        //     for(;;)
        //     {
        //         /* Get the command string length
        //         If receive fails, the client most likely exited */
        //         if(receive(currSd, (char *)&netLen, sizeof(netLen)))
        //             break;
        //         /* Convert from network byte order */
        //         len = ntohl(netLen);
        //         command = malloc(len+1);
        //         /* Get the command and write terminator */
        //         receive(currSd, command, len);
        //         command[len] = 0;
        //         /* Execute the command and get the answer character string */    
        //         if(strcmp(command,"help") == 0)
        //             answer = strdup(
        //                 "server is active.\n\n"
        //                 "    commands:\n"
        //                 "       help: print this help\n"
        //                 "       quit: stop client connection\n"
        //                 "       stop: force stop server connection\n"
        //                 );
        //         else if (strcmp(command,"stop") == 0) {
        //             answer = strdup("closing server connection");
        //             exit_status = 1;
        //         }
        //         else 
        //             answer = strdup("invalid command (try help).");
        //         /* Send the answer back */
        //         len = strlen(answer);
        //         /* Convert to network byte order */
        //         netLen = htonl(len);
        //         /* Send answer character length */
        //         if (send(currSd, &netLen, sizeof(netLen), 0) == -1)
        //             break;
        //         /* Send answer characters */
        //         if (send(currSd, answer, len, 0) == -1)
        //             break;
        //         free(command);
        //         free(answer);
        //         if (exit_status)  {
        //             break;
        //         }
        //     }
        // }


        // printf("End of debug\n");

        // DEBUG-END ===================================================
        #pragma endregion

        unsigned int id = 0, active_threads = 0;
        int routine, action, ret_id;
        int close_client = 0;
        struct thread threads[MAX_THREADS];
        struct thread th_analysis[MAX_THREADS];

        bzero(threads, MAX_THREADS * sizeof(struct thread));
        bzero(th_analysis, MAX_THREADS * sizeof(struct thread));

    
        while(!close_client){
            close_client = 0;
            ret_id = listen_for_commands(c_socket, &routine, &action);

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
                if(active_threads+1 > MAX_THREADS){
                    printf("[i] Max number of thread reached\n");
                    send_data(c_socket, "Function not started - max number of threads reached");
                    continue;
                }
                if(existing_id(threads, active_threads, id)){
                    send_data(c_socket, "Function not started - id already used: Use a different id");
                    continue;
                }
                memcpy(th_analysis, threads, MAX_THREADS * sizeof(struct thread));
            }

            if(routine > CLOSE && action == END)
                if(!existing_id(threads, active_threads, id)){
                    printf("[-] Thread not existing\n");
                    send_data(c_socket, "No action performed - id does not exist");
                    continue;
                }
            
            
            // routine and actions are correct (no further checks need to be done)
            switch(routine){
                case CLOSE: 
                    send_data(c_socket, "bye");
                    close_client = 1;
                break;
                case HELP: 
                    send_data(c_socket, help);
                    continue;
                break;
                case READ: 
                    switch(action){
                        case START:
                            if(existing_id(threads, active_threads, id)){
                                printf("[x] Already existing id\n");
                                send_data(c_socket, "ID already existing");
                                continue;
                            }

                            period = 20;
                            comptime = 2;
                            priority = 1;
                            type = READ;

                            struct thread new_th = {{id++, period, comptime, priority, type}};
                            th_analysis[active_threads] = new_th;

                            if(!is_schedulable(th_analysis, active_threads+1)){
                                printf("[x] Non schedulable thread\n");
                                send_data(c_socket, "Function not started - not schedulable thread");
                                continue;
                            }

                            if(pthread_create(new_th.thread, NULL, &read_datac,NULL)){
                                printf("[x] Error while creating a new thread\n");
                                send_data(c_socket, "Function not started - error while creating a new thread");
                                continue;
                            }
                            if(!pthread_setschedprio(new_th.thread, priority))
                                printf("[!] Can't set the priority to the new thread\n");
                            
                            threads[active_threads++] = new_th;

                        break;

                        case END:
                            int j; 
                            for(j=0; j<active_threads && threads[j].info.id != id; j++);

                            if(pthread_cancel(threads[j].thread)){
                                printf("[!] It was not possible to stop the thread\n");
                                send_data(c_socket, "Function not stopped - thread was not stopped due to an error");
                                continue;
                            }

                            // Compat other active threads
                            for(; j<active_threads-1; j++)
                                threads[j] = threads[j+1];
                            
                            active_threads--;

                            printf("[+] Thread correctly stopped\n");
                            send_data(c_socket, "Function correclty stopped");
                        break;

                        default:
                            printf("[x] thread %s: action not recognized\n", ROUTINES[READ]);
                            send_data(c_socket, "start/end command not recognize");
                    }
                break;

                case WRITE: 
                break;

                case SEND: 
                break;

                default:
                    printf("[-] Invalid routine\n");
                    send_data(c_socket, "Invalid routine");
            }
        }

        // Kill all remaining zombies
        for(int i=0; i<active_threads; i++)
            pthread_cancel(threads[i].thread);
    }

    return 0;
}


void read_datac(){
    int computation = 2;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: read_data\n");
}

void store_datac(){
    int computation = 4;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: store_data\n");
}

void send_datac(){
    int computation = 20;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: send_data\n");
}

int existing_id(struct thread ths[], int active_th, int id){
    for(int i=0; i<active_th; i++)
        if(ths[i].info.id == id)
            return 1;
    return 0;
}
