#define PROC

#include <stdlib.h>
#include <stdio.h>
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
    while ((newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen))) {
#ifndef PROC
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL,  connection_handler, (void*) &newsockfd) < 0)
            error("could not create thread");
        //pthread_join(thread_id, NULL);
#else
        int pid = fork();
        if (pid < 0) { // error occurred
            error("Fork ERROR!");
        } else if (pid == 0) { // child process
            connection_handler((void*)&newsockfd);
            exit(0);
        }
#endif
    }

    shutdown(sockfd, 2);
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

    while ((read_size = read(sock, command, COMMAND_LEN - 1)) > 0) {
        if (read_size < 0)
            error("ERROR reading from socket");

        FILE *fp;
        char path[1035];

        fp = popen(command, "r");
        if (fp == NULL) {
            error("Failed to run command\n");
        }

        while (fgets(path, sizeof(path) - 1, fp) != NULL) {
            write(sock, path, strlen(path));
        }

        shutdown(sock, 1);
        pclose(fp);
    }
    pthread_exit(NULL);
}

