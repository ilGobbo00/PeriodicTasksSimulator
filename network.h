#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
    Function to initialize and run the server

    @param int port for listen incoming connection
    @return int socket, -1 on error
*/
int create_server(int);

/*
    Listen for incoming connection once the server is active. 
    It checks for the schedulability of the tasks and eventually starts new threads

    @param int socket for connection 
    @param int port for listening connection
    @return int -1 on error, 0 otherwise
*/
int conn_listen(int, int);

#endif