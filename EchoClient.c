#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include "udp.h"

void getParameter(int *max_retries, char *sent, char *host_name, int *port_number);
Result monitorSocket(int socket_fd, int retries);
Result Echo (char *message, char *replymessage, int s, struct sockaddr_in server, int maxretries);

void main()
{
	int sock_fd, port, max_retries;
    char host[HOST_SIZE];
    char sent[DATA_SIZE], received[DATA_SIZE];
    Result state;
	struct sockaddr_in client, server;

    memset(received, 0, sizeof(received));
    memset(sent, 0, sizeof(sent));

    getParameter(&max_retries, sent, host, &port);

	// Leaving program when fail to create socket
	if(( sock_fd = socket(AF_INET, SOCK_DGRAM, 0))<0) {
		perror("Could not create socket");
		exit(1);
	}
	
	setServerAddr(&server, host, port);

    state = Echo(sent, received, sock_fd, server, max_retries);
    if(state == FAIL || state == TIMEOUT)
	{	
		printf("Timeout, no response from server. Exiting client..\n");
		exit(1);
    }
    else
        exit(0);
}

Result Echo (char *message, char *replymessage, int sock_fd, struct sockaddr_in server, int maxretries)
{
	Result state=SUCCESS;
    bool exitLoop = false;

	if( (state = sendMessage(&sock_fd, message, &server)) == FAIL){
		printf("Sending message to echo server fail.\n");
        return state;
    }

    printf("Message:  '%s' has been sent.\nWaiting for the echo.\n", message);

    for(int fail_times = 0; fail_times < maxretries && !exitLoop; fail_times++)
    {
        switch (monitorSocket(sock_fd, fail_times)){
            case SUCCESS:
                if((state = receiveMessage(&sock_fd, replymessage, &server)) == FAIL)
                    printf("Receiving message from server fail.\n");
                else
                    printf("Echo message from the echo server:\n'%s'\n", replymessage);
                exitLoop = true;
                break;
            case FAIL:
                printf("Occurring error! Error code: %d.\n", errno);
                exitLoop = true;
                break;
            case TIMEOUT:
                printf("Left retry %d times.\n", maxretries - fail_times - 1);
                sendMessage(&sock_fd, message, &server);
                break;
        }
    }
    return state;	
}


Result monitorSocket(int socket_fd, int retries)
{
	Result state=SUCCESS;
    struct timeval timeout;
    double wait_time = 0.5 * (1 << retries);
    double int_sec, decimal_sec;

    int rc;
	fd_set rfds;

	FD_ZERO(&rfds);
    FD_SET(socket_fd, &rfds);

    wait_time > 8.0 ? wait_time = 8.0 : wait_time;

    decimal_sec = modf(wait_time, &int_sec);
	timeout.tv_sec = (int) int_sec;
	timeout.tv_usec = (int) (decimal_sec * 1000000);

	if((rc = select(socket_fd + 1, &rfds, 0, 0, &timeout))<0){
        perror("Select fail:\n");
        state=FAIL;
    }
	else if( rc == 0)
	{
		state=TIMEOUT;
        printf("Timed out for %.2f sec, no response from server\n", wait_time);
	}
	return state;		
}

void getParameter(int *max_retries, char *sent, char *host_name, int *port_number)
{
    getString("Please enter the IPv4 address of server:", host_name);

    getValidNumber("Please enter the port number of server:", port_number);
    getValidNumber("Please enter the maximum retry times:", max_retries);

    getString("Please enter the message to be sent to server:", sent);
}
