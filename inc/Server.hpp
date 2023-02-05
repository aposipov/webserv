//
// Created by user on 08.12.22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#define SERVER_PORT 8080

#define SIZE_OF_BUF 4096

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//#include <sys/event.h>
#include <sys/time.h>

#include "Conf.hpp"
#include "Client.hpp"


class Server
{

	public:


	Server(Conf const &);
	Server(Server const &rhs);
	~Server();

	int	add_new_client(int const accept_fd);

	int	get_request(int connfd = 0);

	int	manage_request(int connfd);

	std::vector<int> const	&get_listen_fd() const;

	uint32_t	inet_atonl(std::string const &addr) const;

	int	action_response(int connfd);



	private:

	Server();
	int	 manage_get(Client &client);
	std::string	choose_path(std::string req_path);


	std::vector<int>		listen_fd;
	Conf const				&my_config;
	struct sockaddr_in		servaddr;
	std::map<int, Client>	clients;
	std::size_t				buf_size;


};

#endif //WEBSERV_SERVER_HPP
