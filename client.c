#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "defines.h"

void error(char *msg);


int main(int argc, char const *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    portno = PORTNO;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname("127.0.0.1");
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

	while (true) {
        char buf[CLIENT_RESP_LEN];

        printf("Enter command:");
        bzero(buffer, COMMAND_LEN);
        fgets(buffer, COMMAND_LEN - 1,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");

        while (read(sockfd, buf, sizeof(buf)-1))
            printf("%s", buf);
	}
	return 0;
}


void error(char *msg)
{
    perror(msg);
    exit(0);
}