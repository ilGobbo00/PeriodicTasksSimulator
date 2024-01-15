#include "network.h"


int create_server(int port){
    int s;
    int len;
    struct sockaddr_in sin;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return -1;
    }

    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    #ifdef SO_REUSEPORT
    if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
        perror("setsockopt(SO_REUSEPORT) failed");
    #endif
    // Initialize the struct
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    // Bind port and socket
    if (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1){
        perror("bind");
        return -1;
    }

    return s;
}

int conn_listen(int socket, int port){
    if(listen(socket, 1) == -1){
        perror("listen");
        return -1;
    }

    int c_socket;
    struct sockaddr_in incoming_c;
    unsigned int addrLen = sizeof(incoming_c);

    if ((c_socket = accept(socket, (struct sockaddr *) &incoming_c, &addrLen)) == -1){
        perror("accept");
        return -1;
    }

    printf("Connection received from %s\n", inet_ntoa(incoming_c.sin_addr));
    return c_socket;
}

int send_data(int sd, char* buffer){
    unsigned int bufsize = htonl(strlen(buffer));

    // Send length
    if(send(sd, &bufsize, sizeof(bufsize), 0) == -1)
        return -1;

    // Send data and return status code
    return send(sd, buffer, strlen(buffer), 0);
}

int read_resp(int sd, char *buff, int maxsize){
    // Clear the buffer
    bzero(buff, maxsize);      

    // Read the incoming data
    unsigned int t=0, rlen;

    // Get the amount of data to read
    if(read(sd, &rlen, sizeof(rlen)) == -1){
        perror("read");
        return -1;
    }
    rlen = ntohl(rlen);

    // String is already terminated due to bzero

    // Read data and return status code
    return read(sd, buff, rlen > maxsize ? maxsize : rlen);
}

int is_routine(char *r){
    for(int i=0; i<N_ROUTINES; i++)
        if(!strcmp(ROUTINES[i], r))
            return 1;

    return 0;
}

int listen_for_commands(int sd, int* routine, int* action){
    char client_comm[20];
    char r[10], a[10];

    int id = NONE;

    // Read response
    if(read_resp(sd, client_comm, 20) == -1){
        printf("Error while reading client request\n");
        return -1;
    }

    printf("[i] Log: %s\n", client_comm);

    // Check for syntax
    sscanf(client_comm, "%s %s %d", r, a, &id);
    if(strlen(client_comm) && !is_routine(r)){
        printf("Wrong client input\n");
            return WRONG; 
    }

    // Set action
    if(!strcmp(a, "start")) *action = START;
    else if(!strcmp(a, "end")) *action = END;
    else *action = ERROR;

    // Set routine
    if(!strcmp(r, ROUTINES[CLOSE]) || !strlen(client_comm)){
        *routine = CLOSE;
        return NONE;
    }
    
    if(!strcmp(r, ROUTINES[HELP])){
        *routine = HELP;
        return NONE;
    }

    if(!strcmp(r, ROUTINES[READ]))
        *routine = READ;
        
    if(!strcmp(r, ROUTINES[WRITE]))
        *routine = WRITE;

    if(!strcmp(r, ROUTINES[SEND]))
        *routine = SEND;

    return !strcmp(a, ACTIONS[START]) ? id : NONE;
}