#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "udp.h"

void setReceiverAddr(struct sockaddr_in *receiver, const char *ip_address, int port)
{
    receiver->sin_family = AF_INET;
    receiver->sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &(receiver->sin_addr));
}

void setServerAddr(struct sockaddr_in * server, char *hostname, int port)
{
    server->sin_family = AF_INET;
    inet_pton(AF_INET, hostname, &server->sin_addr);
    server->sin_port = htons(port);
}

void getValidNumber(const char *prompt, int *value)
{
    printf("%s\n", prompt);
    while (1) {
        /* If successfully convert input to integer, break the loop.
            Otherwise, print error message and continue loop until input is valid */
        int error = scanf("%d", value);
        while (getchar() != '\n');
        if (error == 1)
            break;
        printf("Invalid input format. Please enter a valid integer.\n");
    }
}

void getString(const char *prompt, char *string)
{
    printf("%s\n", prompt);
    fgets(string, DATA_SIZE, stdin); 

    char *end_position;
    if((end_position = strchr(string, '\n')) != NULL)
        *end_position = '\0';
}

Result sendMessage(int *sock_fd, char *message, struct sockaddr_in *server)
{
	Result state;
    int rc;
	if( (rc = sendto(*sock_fd, message, strlen(message), 0, (struct sockaddr *)server, sizeof(*server))) > 0)
		state=SUCCESS;	
	else 
	{
        printf("%d\n", errno);
		printf("Fail to send message.\n");
		state=FAIL;
	}

	return state;
}

Result receiveMessage(int *sock_fd, char *message, struct sockaddr_in *server)
{
	Result state;
    int addr_len=sizeof(struct sockaddr_in);
    int rc;
	if((rc = recvfrom(*sock_fd, message, DATA_SIZE, 0, (struct sockaddr *)server, &addr_len)) > 0) 
		state=SUCCESS;
	else
	{
		printf("Fail to receive message.\n");
		state=FAIL;
	}

	return state;
}
