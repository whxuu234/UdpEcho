#ifndef __UDP_H__
#define __UDP_H__

#ifndef __UDP__
#define __UDP__
#endif

#define DATA_SIZE 1024
#define HOST_SIZE 100

typedef enum
{
	SUCCESS,
	FAIL,
    TIMEOUT

} Result;

/******************************************************************************
 *                                                                            *
 *                              UDP Functions                                *
 *                                                                            *
 ******************************************************************************/

void setReceiverAddr(struct sockaddr_in *receiver, const char *ip_address, int port);
void setServerAddr(struct sockaddr_in * server, char *hostname, int port);
void getValidNumber(const char *prompt, int *value);
void getString(const char *prompt, char *string);
Result sendMessage(int *sock_fd, char *message, struct sockaddr_in *server);
Result receiveMessage(int *sock_fd, char *message, struct sockaddr_in *server);


#endif // __UDP__