#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "defines.h"

void error(char *msg);
void *connection_handler(void *);


int main(int argc, char const *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    portno = PORTNO;

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd,5);

    clilen = sizeof(cli_addr);
    while(!(newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) < 0)
    {
        pthread_t thread_id;
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &newsockfd) < 0)
         error("could not create thread");
    }


    return 0;
}


void error(char *msg)
{
    perror(msg);
    exit(1);
}


void *connection_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    int read_size;
    char command[COMMAND_LEN];

    read_size = read(sock, command, COMMAND_LEN - 1);
    if (read_size < 0)
        error("ERROR reading from socket");

    pthread_exit(NULL);
}

