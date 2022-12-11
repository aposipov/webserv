//
// Created by user on 08.12.22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#define SERVER_PORT 8080

#include <iostream>
#include <map>
#include <vector>

#include <unistd.h>
#include <cstring> //add for fedora

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

	private:

	Server();
	Server(Server const &rhs);

	std::map<std::string, std::vector<std::string> >	config_table;
	int													listen_fd;
	struct sockaddr_in									servaddr;


};

#endif //WEBSERV_SERVER_HPP
