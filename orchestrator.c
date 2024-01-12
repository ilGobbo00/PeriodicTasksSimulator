#include "scheduling.h"
#include "network.h"

// Routines 
void read_data();
void store_data();
void send_data();


int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage:\n\t%s <port between 1024 and 65535>\n", argv[0]);
        return 1;
    }

    int port, socket;
    sscanf(argv[1], "%d", &port);

    if((socket = create_server(port)) == -1){
        printf("Error while creating server\n");
        return 1;
    }

    if(conn_listen(port, socket) == -1){
        printf("Error while listening for incoming connections\n");
        return 1;
    } 

    int active_threads = 0;
    struct thread threads[MAX_THREADS];

    // TODO once the client is connected, continue to listen for task activation/deactivation 

    
    return 0;
}


void read_data(){
    int computation = 2;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: read_data\n");
}

void store_data(){
    int computation = 4;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: store_data\n");
}

void send_data(){
    int computation = 20;
    const struct timespec unit = {0,850000}; 

    while(computation--)
        if(nanosleep(&unit, NULL))
            printf("Nanosleep error: send_data\n");
}