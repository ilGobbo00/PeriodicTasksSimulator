#ifndef NETWORK_H
#define NETWORK_H

#include "scheduling.h"
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

    @param int socket for connection 
    @param int port for listening connection
    @return int -1 on error, 0 otherwise
*/
int conn_listen(int, int);

/*
    Function to read incoming data

    @param int socket
    @param char* buffer to store read data
    @param int amount of data to read
    @return bytes read or -1 on error
*/
int read_resp(int, char*, int);

/*
    Function to send data

    @param int socket
    @param char* buffer containin data to send
    @return int bytes sent or -1 on error
*/
int send_data(int, char*);

/*
    Listen for commands coming from a client already connected. It checks for all the possible errors that may happen
    @param int socket
    @param int* represents the target routine
    @param int* represents the action (start/end)
    @return int -1 on errors, -2 no action required, -3 wrong command
*/
int listen_for_commands(int sd, int* routine, int* action);

#endif