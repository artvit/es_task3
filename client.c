#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

void error(char *msg);


int main(int argc, char const *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

	while (true) {
		printf("Enter command:");
	}
	return 0;
}


void error(char *msg)
{
    perror(msg);
    exit(0);
}