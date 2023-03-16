//
// Created by user on 08.12.22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <sys/time.h>
#include <ctime>

#include "Conf.hpp"
#include "Client.hpp"

#define BUFFER_SIZE 4096

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


	int	action_response(int connfd);

	int	check_timeout(int connfd);

	std::string	get_my_name() const;

	int	erase_client(int connfd);

	private:

	Server();
	
	uint32_t	inet_atonl(std::string const &addr) const;
	int			manage_get(Client &client);
	int			manage_post(Client &client);
	int			manage_delete(Client &client);
	int			run_cgi(Client &client);
	int			add_cgi_variables(Client &client, std::vector<std::vector<char> > &envp);
	int			run_post_cgi(Client &client);

	std::string	get_socket_port(int fd) const;
	std::string get_client_addr(int fd) const;

	int choose_path(Response &response, std::string const &method);
	int	set_settings(Client &client, std::map<std::string, Location> const &loc);
	std::string	get_only_name(std::string path) const;


	std::vector<int>		listen_fd;
	Conf const				&my_config;
	struct sockaddr_in		servaddr;
	std::map<int, Client>	clients;
	std::size_t				buf_size;


};

#endif //WEBSERV_SERVER_HPP
