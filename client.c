#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define SERVER_PORT 80

#define MAXLINE 4096
#define SA struct sockaddr

void	app_error(char *str)
{
	printf("%s\n", str);
	exit(1);
}

int main(int argc, char **argv)
{
	int sockfd, n;
	int sendbytes;
	struct sockaddr_in servaddr;
	char sendline[MAXLINE];
	char recvline[MAXLINE];

	if (argc != 2)
		app_error("Usage: CLIENT <server address>");
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0))
		app_error("Error: cannot create socket");
	printf("Socket fd: %d\n", sockfd);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		app_error("Error: inet_pton failure");



	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
		app_error("Error: cannot connect");

	// We're connect. Prepare the message.
	sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
	sendbytes = strlen(sendline);

        // Send the request -- Making sure you send it all
	// This code is a bit fragile, since it bails if only some of the bytes are send.
	// normally, you would want to retry , unless the return value was -1.
	if (write(sockfd, sendline, sendbytes) != sendbytes)
		app_error("Error: could not write to file descriptor");
	memset(recvline, 0, MAXLINE);
	// Now read the server response.
	while ((n = read(sockfd, recvline, MAXLINE - 1) > 0))
	{
		printf("%s\n", recvline);
	}
	if (n < 0)
		app_error("Error: Cound not read from file descritor");
	exit(0);
	return (0);
}
