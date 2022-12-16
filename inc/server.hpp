//
// Created by user on 08.12.22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#define SERVER_PORT 8080
#define SIZE_OF_BUF 4096

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <unistd.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Server
{

	public:

	Server(std::map<std::string, std::vector<std::string> > &config_table);
	~Server();

	int	request();

	int	get_my_pid() const;

	int	manage_request();


	private:

	Server();
	Server(Server const &rhs);

	int													listen_fd;
	int													connfd;	
	int													pid;
	std::map<std::string, std::string>					Header_of_request;
	std::map<std::string, std::vector<std::string> >	config_table;
	struct sockaddr_in									servaddr;


};

#endif //WEBSERV_SERVER_HPP
