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
/* Receive routine: use recv to receive from socket and manage
   the fact that recv may return after having read less bytes than
   the passed buffer size
   In most cases recv will read ALL requested bytes, and the loop body
   will be executed once. This is not however guaranteed and must
   be handled by the user program. The routine returns 0 upon
   successful completion, -1 otherwise */
// static int receive(int sd, char *retBuf, int size){
//     int totSize, currSize;
//     totSize = 0;
//     while (totSize < size){
//         currSize = recv(sd, &retBuf[totSize], size - totSize, 0);
//         if (currSize <= 0)
//             /* An error occurred */
//             return -1;
//         totSize += currSize;
//     }
//     return 0;
// }

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



/* Main client program. The IP address and the port number of
   the server are passed in the command line. After establishing
   a connection, the program will read commands from the terminal
   and send them to the server. The returned answer string is
   then printed. */
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
    // printf("%d.%d.%d.%d\n", ((unsigned char*)server)[0], ((unsigned char*)server)[1],((unsigned char*)server)[2],((unsigned char*)server)[3]);

    /* fill in the socket structure with host information */
    //   memset(&socket, 0, sizeof(socket));
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
        scanf("%s", data);

        if (!strcmp(data, "quit"))
            break;
        
        // Send user command
        if((res = send_data(s, data)) == -1){
            printf("[!] Some error occured while sending data\n");
            continue;
        }
        // else
            // printf("[+] Correctly sent %d bytes\n", res);

        if((res = read_resp(s, data, datalen)) == -1)
            printf("[!] Some error occured while reading data\n");
        else
            printf("[<] %s\n", data);

        if(!strcmp(data, "bye")) break;
        // DEBUG
/* 
        // Send first the number of characters in the command and then
        //    the command itself
        len = strlen(command);
        // Convert the integer number into network byte order
        netLen = htonl(len);

        // Send number of characters
        if (send(sd, &netLen, sizeof(netLen), 0) == -1){
            perror("send");
            exit(1);
        }
        // Send the command
        if (send(sd, command, len, 0) == -1){
            perror("send");
            exit(0);
        }
        // Receive the answer: first the number of characters
        //    and then the answer itself
        if (receive(sd, (char *)&netLen, sizeof(netLen))){
            perror("recv");
            exit(0);
        }
        // Convert from Network byte order
        len = ntohl(netLen);
        // Allocate and receive the answer
        answer = malloc(len + 1);
        if (receive(sd, answer, len)){
            perror("recv");
            exit(1);
        }
        answer[len] = 0;
        printf("%s\n", answer);
        free(answer);
        if (!strcmp(command, "stop"))
            break; 
        */
    
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