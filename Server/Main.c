/*
 * Main.c
 *
 *  Created on: Jan 28, 2013
 *      Author: student
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#define NCLIENTS 100

typedef struct Client {
	char *client_id;
	pthread_t thread_id;
} client;

void error(char *msg)
{
	perror(msg);
	exit(1);
}

void dostuff (int *sock)
{
	int n;
	char buffer[256];

	bzero(buffer, 256);
	n = recv(*sock, buffer, 255, NULL);

	if (n < 0)
		error("ERROR: Cannot read socket");

	printf("Message: %s\n", buffer);
	n = send(*sock, "Message received", 18, NULL);

	if (n < 0)
		error("ERROR: Cannot write to socket");
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;

	client clients[NCLIENTS];
	pthread_attr_t attr;
	int client_num = 0;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if (argc < 2)
		error("ERROR: No port provided");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR: Cannot open socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR: Cannot bind to socket");

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	while(1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0)
			error("ERROR: Cannot accept socket");

		pthread_create(&clients[client_num++]->thread_id, &attr, &dostuff, (void *) &newsockfd);
	}

	return 0;
}
