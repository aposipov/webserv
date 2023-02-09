/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 19:37:29 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/09 10:39:56 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(Conf const &conf) : listen_fd(), my_config(conf)
{
	for (Conf::listen_t::const_iterator it = conf.listen.begin(); it != conf.listen.end(); ++it)
	{
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = PF_INET;
		servaddr.sin_addr.s_addr = htonl(inet_atonl(it->first));std::cout << it->first << "-->" << inet_atonl(it->first) << std::endl;
		for (std::vector<std::string>::const_iterator ite = it->second.begin(); ite < it->second.end(); ++ite)
		{
			std::istringstream iss(*ite);
			int port;
        	iss >> port;
			servaddr.sin_port = htons(port);
			listen_fd.push_back(socket(PF_INET, SOCK_STREAM, 0));
			if (listen_fd.back() < 0)
				std::cerr << "Socket error" << std::endl;//exception
			int opt = 1;
			std::cout << setsockopt(listen_fd.back(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
			std::cout << " - " << opt << std::endl;
			if ((bind(listen_fd.back(), (const struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
				throw(std::logic_error("Bind error"));
			if ((listen(listen_fd.back(), SOMAXCONN)) < 0)
				std::cerr << "Listen error" << std::endl;//exception
			fcntl(listen_fd.back(), F_SETFL, O_NONBLOCK);
		}
	}
	std::cout << "Server is created!" << std::endl;
}

Server::Server(Server const &rhs) : listen_fd(rhs.listen_fd), my_config(rhs.my_config), servaddr(rhs.servaddr)
{
	std::cout << "Server is copied" << std::endl;

}

Server::~Server()
{
	std::cout << "Server finished" << std::endl;
}


uint32_t Server::inet_atonl(std::string const &addr) const
{
    std::istringstream iss(addr);
    
    uint32_t ipv4 = 0;
    
    for( uint32_t i = 0; i < 4; ++i ) {
        uint32_t part;
        iss >> part;
        if ( iss.fail() || part > 255 ) {
            throw std::runtime_error( "Invalid IP address - Expected [0, 255]" );
        }
        
        // LSHIFT and OR all parts together with the first part as the MSB
        ipv4 |= part << ( 8 * ( 3 - i ) );

        // Check for delimiter except on last iteration
        if ( i != 3 ) {
            char delimiter;
            iss >> delimiter;
            if ( iss.fail() || delimiter != '.' ) {
                throw std::runtime_error( "Invalid IP address - Expected '.' delimiter" );
            }
        }
    }
    
    return ipv4;
}

int	Server::add_new_client(int const accept_fd)
{
	std::pair<int, Client> tmp;
	socklen_t len = sizeof(Client::client_t);

	tmp.first = accept(accept_fd, tmp.second.getClientAddr(), &len);
	if (tmp.first <= 0)
		return -1;
	tmp.second.set_myFd(tmp.first);
	tmp.second.setTimeout(my_config.keep_time_alive);
	clients.insert(tmp);
	std::cout << "Accepted new client: " << clients.at(tmp.first).get_myFd() << " by server " << accept_fd << std::endl;
	fcntl(tmp.first, F_SETFL, O_NONBLOCK);
	return (tmp.first);
}

std::vector<int> const	&Server::get_listen_fd() const { return listen_fd; }

int	Server::check_timeout(int connfd)
{
	Client	&client = this->clients.at(connfd);
	ssize_t current_time = std::time(nullptr);

	if (current_time > client.getTimeout())
	{
		clients.erase(connfd);
		close(connfd);
		return connfd;
	}
	return 0;
}

int	Server::get_request(int	connfd)
{
	int			n = 0;
	char		*buf = new char[my_config.client_body_buffer_size];
	Client		&client = this->clients.at(connfd);
	std::string	&Buf = client.messageRef();
	std::cout << "Begin getting request from client: " << client.get_myFd() << std::endl;

	memset(buf, 0, my_config.client_body_buffer_size);
	while ((n = recv(connfd, buf, my_config.client_body_buffer_size - 1, 0)) > 0)
	{
		Buf.append(buf, n);
		std::cout << Buf << std::endl;
	}
	std::cout << "Got available request from client: " << client.get_myFd() << " and n is " << n << std::endl;
	delete [] buf;
	if (Buf.find("POST") == 0)
	{
		std::size_t req_lenght = Buf.find("\r\n\r\n"), mess_lenght = Buf.find("Content-Length:");
		if (req_lenght == std::string::npos)
			return 0;
		if (mess_lenght == std::string::npos)
		{
			//send wrong request
			Buf.clear();
			return (-1);
		}
		if (Buf.size() - req_lenght - 4 < (std::size_t)std::atol(Buf.substr(mess_lenght + 15).c_str()))
			return 0;
	}
	else if (Buf.size() < 4 || (Buf.size() > 3 && Buf.substr(Buf.size() - 4) != "\r\n\r\n"))
		return 0;
	Request	tmp(Buf);
	client.setRequest(tmp);
	Buf.clear();
	client.setTimeout(this->my_config.keep_time_alive);
	return (1);
}

std::string	Server::choose_path(std::string req_path)
{
	//this->my_config.root = root from config file
	//this->my_config.locations = table with type std::map<std::string, Location>
	//check all of locations and root and create a path based on them

	if (req_path == "/" && my_config.autoindex == true)
		req_path += my_config.index;

	return my_config.root + req_path;
}

int	 Server::manage_get(Client &client)
{
	Request const &request = client.getReqest();
	std::string &page = client.getResponseToSet().getContentToFill();

	std::string path = this->choose_path(request.getHeader("Path").first);

	std::ifstream	ifs;
	std::string 	dst;

	std::cout << "Requested file: " << path << std::endl;

	page.append("Server: " + this->my_config.server_name);
	ifs.open(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!ifs.is_open())
		client.getResponseToSet().error_response(404);
	else
	{
		std::streampos size;
  		char *memblock;
		size = ifs.tellg();
    	memblock = new char [size];
    	ifs.seekg (0, std::ios::beg);
    	ifs.read (memblock, size);
		std::stringstream ss;
		ss << size;
		std::string sz = ss.str();
		if (request.getHeader("Path").first.find(".jpg") != std::string::npos)
			page.append("Content-Type: image/jpeg\r\n");
		else 
			page.append("Content-Type: text/html\r\n");
		page.append("Content-Length: " + sz + "\r\n");
		page.append("Accept-Ranges: bytes\r\n");
		if (request.getHeader("Connection").second && request.getHeader("Connection").first == "close")
			page.append("Connection: close\r\n");
		else
			page.append("Connection: keep-alive\r\n");
		page.append("\r\n");
		page.append(memblock, size);
		ifs.close();
		delete [] memblock;
	}
	return 0;
}

int	Server::manage_request(int connfd)
{
	Client	&client = clients.at(connfd);
	std::string tmp = client.getReqest().getHeader("Protocol").first;
	std::pair<std::string, bool>	method = client.getReqest().getHeader("Method");
	if (method.second == false || tmp != "HTTP/1.1")
		client.getResponseToSet().error_response(400);
	else if (method.first == "GET")
		this->manage_get(client);
	else if (method.first == "POST")
		;
	else if (method.first == "DELETE")
		;
	else
		client.getResponseToSet().error_response(400);
	client.clearRequest();
	// client.requestToString();
	return (connfd);
}

int	Server::action_response(int connfd)
{
	Client	&client = clients.at(connfd);
	Response &response = this->clients.at(connfd).getResponseToSet();
	std::string &content = response.getContentToFill();
	ssize_t		sended_size = 0;

	std::cout << "Hello from action_response " << client.get_myFd() << " " << content.size() << std::endl;

	//здесь на основе response обрабатывается ответ
	if (content.size())
	{
		sended_size = send(connfd, content.c_str() + response.getSendedSize(),
			((content.size() - response.getSendedSize()) > my_config.client_body_buffer_size) ? my_config.client_body_buffer_size :
				(content.size() - response.getSendedSize()) , MSG_DONTWAIT);
		std::cout << sended_size << "<<<<<<<<<<<<>>>>>>>>>>>>" << std::endl;
		if (sended_size > 0)
		{
			response.setSendSize(sended_size + response.getSendedSize());
			if (response.getSendedSize() - content.size() == 0)
			{
				content.clear();
				response.setSendSize(0);
			}
		}
	}
	return (0);
}


