#include "network.h"


int create_server(int port){
    int s;
    int len;
    struct sockaddr_in sin;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return -1;
    }

    // Initialize the struct
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    // Bind port and socket
    if (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1)
        return -1;

    return s;
}

int conn_listen(int socket, int port){
    if(listen(socket, 1) == -1)
        return -1;

    int res, addr_len;
    struct sockaddr_in incoming_c;

    if ((res = accept(socket, (struct sockaddr *) &incoming_c, &addr_len)) == -1)
        return -1;
        
    printf("Connection received from %s\n", inet_ntoa(incoming_c.sin_addr));

    return 0;
}