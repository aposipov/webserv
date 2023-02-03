
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
			std::cout << setsockopt(listen_fd.back(), SOL_SOCKET, SO_REUSEADDR | SO_KEEPALIVE, &opt, sizeof(opt));
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
	clients.insert(tmp);
	std::cout << "Accepted new client: " << clients.at(tmp.first).get_myFd() << std::endl;
	// fcntl(tmp.first, F_SETFL, O_NONBLOCK);
	return (tmp.first);
}

std::vector<int> const	&Server::get_listen_fd() const
{
	return listen_fd;
}

int	Server::get_request(int	connfd)
{
	int			n = 0;
	char		buf[SIZE_OF_BUF];
	Client		&client = clients.at(connfd);
	std::string	&Buf = client.messageRef();
	std::cout << "Begin getting request from client: " << client.get_myFd() << std::endl;

	memset(buf, 0, SIZE_OF_BUF);
	while ((n = recv(connfd, buf, SIZE_OF_BUF - 1, 0)) > 0)
	{
		Buf.append(buf, n);
		std::cout << Buf << std::endl;
	}
	std::cout << "Got available request from client: " << client.get_myFd() << std::endl;
	if (connfd == 0 && n > 0)
	{
		std::cin >> Buf;//можно читать стандартный ввод и настраивать вебсервер, обновлять файл конфигурации и т.п.
		return (0);
	}
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
	else if (Buf.size() > 3 && Buf.substr(Buf.size() - 4) != "\r\n\r\n")
		return 0;
	Request	tmp(Buf);
	client.setRequest(tmp);
	Buf.clear();
	Buf.shrink_to_fit();
	return (1);
}

int	Server::manage_request(int connfd)
{
	Client	&client = clients.at(connfd);
	
	std::cout << "Hello from manage_request " << client.get_myFd() <<std::endl;

	// Здесь будет инициализирован response внутри сервера на основе request сервара
	


	return (connfd);
}

int	Server::action_response(int connfd)
{
	Client	&client = clients.at(connfd);

	std::cout << "Hello from action_response " << client.get_myFd() <<std::endl;

	//здесь на основе response обрабатывается ответ

		// std::string response = "HTTP/1.1 200 OK\nContent-Length: ";
	// std::string content = "<!DOCTYPE html><html><head><title>Example</title></head><body><p>This is an example of a simple HTML page with one paragraph.</p></body></html>";
	// std::ifstream ifs;
	// ifs.open(Buf.substr(Buf.find('/')+1, Buf.find(' ', Buf.find('/')) - Buf.find('/')-1));
	// if (ifs.is_open())
	// {
	// 	std::string dst;
	// 	while (std::getline(ifs, dst))//Для передачи файла, можно поставить std::binary mode и читать через ifs.read()
	// 	{
	// 		response.append(dst);
	// 	}
	// 	// std::cout << response;
	// 	ifs.close();
	// }
	// else
	// 	response = content + 143 + "\r\n\r\n";
	// send(connfd, response.c_str(), response.size(), MSG_DONTWAIT);

	return (0);
}


