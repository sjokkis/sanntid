/*
 * Main.c
 *
 *  Created on: Jan 28, 2013
 *      Author: student
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;

	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];

	if (argc < 3)
		error("ERROR: Wrong usage");

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR: Cannot open socket");

	server = gethostbyname(argv[1]);

	if (server == NULL)
		error("ERROR: No such host");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR: Cannot connect to socket");

	printf("Please enter message\n");

	while(1) {
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		n = send(sockfd, buffer, strlen(buffer), NULL);

		if (n < 0)
			error("ERROR: Cannot write to socket");

		bzero(buffer, 256);
		n = recv(sockfd, buffer, 255, NULL);

		if (n < 0)
			error("ERROR: Cannot read from socket");

		printf("%s\n", buffer);
	}

	return 0;
}
