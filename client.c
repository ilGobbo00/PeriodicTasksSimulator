#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEBUG printf("--- HERE! ----");

/*
    Function to read incoming data (it comprehends a final CRLF)

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


int main(int argc, char **argv){
    char hostname[100] = "localhost";
    
    int datalen = 500;
    char data[datalen];

    int res;            // Results of calls
    int port;
    int s;                  // Socket
    struct sockaddr_in sd;  // Struct to enstablish connection
    struct hostent *server;

    // char *answer;
    // int len;
    // unsigned int netLen;

    /* Check number of arguments and get IP address and port */
    if (argc < 2){
        printf("Usage:\n\t%s <hostname (default localhost)> <port>\n", argv[0]);
        return -1;
    }

    if (argc == 2){
        sscanf(argv[1], "%d", &port);
    }

    if (argc == 3){
        sscanf(argv[1], "%s", hostname);
        sscanf(argv[2], "%d", &port);
    }

    /* Resolve the passed name and store the resulting long representation
       in the struct hostent variable */
    if ((server = gethostbyname(hostname)) == 0){
        perror("gethostbyname");
        return -1;
    }

    /* fill in the socket structure with host information */
    sd.sin_family = AF_INET;
    sd.sin_port = htons(port);
    sd.sin_addr.s_addr = ((struct in_addr *)(server->h_addr_list[0]))->s_addr; // IP address of the target server

    /* create a new socket */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return -1;
    }

    /* connect the socket to the port and host
       specified in struct sockaddr_in */
    if (connect(s, (struct sockaddr *)&sd, sizeof(sd)) == -1){
        perror("connect");
        return -1;
    }

    // Print the instruction by default (this will test also the message exchange)
    bzero(data, datalen);
    sprintf(data, "help");
    
    // Test connection and display server's commands
    if(send_data(s, data) == -1){
        printf("[!] Error while sendind data. Execution aborted.\n");
        return -1;
    }
    if(read_resp(s, data, datalen) == -1){
        printf("[!] Error while reading data. Execution aborted.\n");
        return -1;
    }
    
    printf("Enter \"quit\" to exit\n");
    printf("\n[<] %s\n", data);

    while (1){
        // Get command from user
        printf("[>] Enter command: ");
        bzero(data, datalen);
        fgets(data, datalen, stdin);
        data[strlen(data)-1] = 0;

        if (!strcmp(data, "quit"))
            break;
        
        // Send user command
        if((res = send_data(s, data)) == -1){
            printf("[!] Some error occured while sending data\n");
            continue;
        }

        if((res = read_resp(s, data, datalen)) == -1)
            printf("[!] Some error occured while reading data\n");
        else
            printf("[<] %s\n", data);

        if(!strcmp(data, "bye")) break;
    }
    
    /* Close the socket */
    close(s);
    printf("[x] Connection closed\n");
    return 0;
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