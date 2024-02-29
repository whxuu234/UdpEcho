#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "udp.h"

void main()
{
	Result state;
	int port, sock_fd;
	struct sockaddr_in serverAddr, clientAddr;
	char host_name[HOST_SIZE];
	char received[DATA_SIZE], reply[DATA_SIZE];

	getString("Please enter the IPv4 address of server:", host_name);
    getValidNumber("Please enter the port number of server:", &port);

	if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0))<0) {
		perror("Could not create socket");
		exit(1);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));
	
	setReceiverAddr(&serverAddr, host_name, port);

	if( bind(sock_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr))< 0){
		perror("Binding address fail.\n");
		close(sock_fd);
		exit(1);
	}

	printf("Successfully running UDP Echo Server, waiting for UDP messages...\n");

	while(1)
	{
		memset(received, 0, sizeof(received));
		memset(reply, 0, sizeof(reply));

		if((state = receiveMessage(&sock_fd, received, &clientAddr)) == FAIL)
			printf("Receiving message from client fail.\n");
		else
			printf("Received Message: %s\n", received);

		strncpy(reply, received, strlen(received));

		if( (state = sendMessage(&sock_fd, reply, &clientAddr)) == FAIL)
			printf("Sending message to client fail.\n");
	}
}
