#include <iostream>

#include <cstring>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h> //add for fedora

#define MAXLINE 4096

#define SERVER_PORT 80


int main(int argc, char const *argv[])
{
	int	socket_fd;
	struct sockaddr_in	servaddr;
	char recieve[MAXLINE];

	if (argc != 2)
		return 1;
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		return 2;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		return 3;
	
	if (connect(socket_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
		return 4;
	std::string request = "GET / HTTP/1.1\r\n\r\n";
	if (write(socket_fd, request.c_str(), request.size()) != request.size())
		return 5;
	while ( (read(socket_fd, recieve, MAXLINE-1)) > 0)
		printf("%s", recieve);
	return 0;
}
