/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 19:37:29 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/26 01:48:01 by mnathali         ###   ########.fr       */
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

std::string	Server::get_my_name() const { return my_config.server_name; }

int	Server::erase_client(int connfd)
{
	this->clients.erase(connfd);
	return 0;
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
	ssize_t current_time = std::time(NULL);

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
	if (n == 0)
		client.setTimeout(-1);
	if (Buf.find("POST") == 0)
	{
		std::size_t req_lenght = Buf.find("\r\n\r\n"), mess_lenght = Buf.find("Content-Length:");
		if (req_lenght == std::string::npos)
			return 0;
		if (mess_lenght == std::string::npos)
		{
			//send wrong request
			Buf.clear();
			client.setTimeout(-1);
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

int Server::choose_path(Response &response, std::string const &method)
{
	std::string	const &req_path = response.getPath();
	struct stat sb;
	std::string path = ((method == "GET") ? response.getSettings().root : response.getSettings().upload_path) + req_path;

	stat(path.c_str(), &sb);

	if (req_path.size() && req_path[req_path.size() - 1] == '/')
		response.setPath(path + response.getSettings().index);
	else if (sb.st_mode & S_IFDIR && response.getSettings().index.size())
		response.setPath(path + "/" + response.getSettings().index);
	else
		response.setPath(path);
	return 0;
}

int	Server::set_settings(Client &client, std::map<std::string, Location> const &loc)
{

	std::string req_path = client.getResponseToSet().getPath();

	for (std::map<std::string, Location>::const_iterator it = loc.begin(); it != loc.end(); ++it)
	{
		if (req_path.find(it->first) == 0 || req_path.find(it->first) == req_path.size() - it->first.size()
			|| (req_path.size() && req_path[req_path.size() - 1] == '/'
			&& client.getResponseToSet().getSettings().index.find(it->first) == client.getResponseToSet().getSettings().index.size() - it->first.size()))
		{
			client.getResponseToSet().setSettings(it->second);
			if (it->second.locations.size())
				set_settings(client, it->second.locations);
			else
				return 1;
		}
	}
	return 0;
}

int	 Server::manage_get(Client &client)
{
	Request const &request = client.getReqest();
	std::string &page = client.getResponseToSet().getContentToFill();
	std::ifstream	ifs;
	std::string 	dst;

	page.append("HTTP/1.1 200 OK\r\n");
	page.append("Server: " + this->my_config.server_name);
	
	if (client.getResponseToSet().getSettings().redirection.first)
	{
		int code = client.getResponseToSet().getSettings().redirection.first;
		if (code % 400)
			client.getResponseToSet().error_response(code, this->my_config.error_pages.find(code)->second);
		else
			client.getResponseToSet().fillRedirection();
		return 0;
	}

	if (client.getResponseToSet().getSettings().cgi_param.size())
	{
		int pid = fork();
		std::cout << "pid = " << pid << std::endl;
		if (pid == 0)
			this->run_cgi(client);
		page.clear();
		sleep(10);
		return 0;
	}
		 
	ifs.open(client.getResponseToSet().getPath().c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!ifs.is_open())
	{
		std::cout << "Not open " << client.getResponseToSet().getSettings().autoindex << " " << client.getResponseToSet().getSettings().methods << std::endl;
		if (!(client.getResponseToSet().getSettings().autoindex && client.getResponseToSet().getSettings().methods > 3))
			if (!client.getResponseToSet().autoindex())
				return 0;
		client.getResponseToSet().error_response(404, this->my_config.error_pages.find(404)->second);
	}
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

int	Server::run_cgi(Client &client)
{
	Request const &request = client.getReqest();
	Response 	&response = client.getResponseToSet();
	std::string path = response.getPath();
	
	char const * argv[3] = {this->my_config.server_name.c_str(), path.c_str(), 0};
	std::vector<std::vector<char> > envp;

	for (std::map<std::string, std::string>::const_iterator it = request.headers_begin(), ite = request.headers_end();
		it != ite; ++it)
	{
		std::string env(it->first + "=" + it->second);
		std::vector<char> tmp(env.begin(), env.end());
		envp.push_back(tmp);
	}
	std::vector<char*> array(envp.size() + 1);
	for (std::vector<char*>::size_type i = 0; i < array.size(); ++i)
		array[i] = envp[i].data();
	array[array.size()] = 0;
	dup2(client.get_myFd(), STDOUT_FILENO);
	send(client.get_myFd(), response.getContentToFill().c_str(), response.getContentToFill().size(), MSG_DONTWAIT);
	if (execve(response.getSettings().cgi_param.c_str(), const_cast<char* const*>(argv), reinterpret_cast<char**>(array.data())))
		std::cerr << "Error with running " << response.getSettings().cgi_param << " " << argv[1] << std::endl;
	exit(0);
	return 0;
}

int Server::manage_post(Client &client)
{
	Request const &request = client.getReqest();
	std::string &page = client.getResponseToSet().getContentToFill();
	std::ofstream ofs;

	ofs.open(client.getResponseToSet().getPath().c_str());

	if (!ofs.is_open())
	{
		client.getResponseToSet().error_response(403, this->my_config.error_pages.find(403)->second);
		return 1;
	}
	ofs << request.getContent();
	ofs.close();
	page.append("HTTP/1.1 200 OK\r\n");
	page.append("Server: " + this->my_config.server_name);
	if (request.getHeader("Connection").second && request.getHeader("Connection").first == "close")
		page.append("Connection: close\r\n");
	else
		page.append("Connection: keep-alive\r\n");
	page.append("\r\n");

	return 0;
}

int Server::manage_delete(Client &client)
{
	Request const &request = client.getReqest();
	std::string &page = client.getResponseToSet().getContentToFill();

	if (std::remove(client.getResponseToSet().getPath().c_str()))
	{
		client.getResponseToSet().error_response(403, this->my_config.error_pages.find(403)->second);
		return 1;
	}
	page.append("HTTP/1.1 200 OK\r\n");
	page.append("Server: " + this->my_config.server_name);
	if (request.getHeader("Connection").second && request.getHeader("Connection").first == "close")
		page.append("Connection: close\r\n");
	else
		page.append("Connection: keep-alive\r\n");
	page.append("\r\n");

	return 0;
}

int	Server::manage_request(int connfd)
{
	Client	&client = clients.at(connfd);
	std::string tmp = client.getReqest().getHeader("Protocol").first;
	std::pair<std::string, bool>	method = client.getReqest().getHeader("Method");
	
	client.getResponseToSet().setPath(client.getReqest().getHeader("Path").first);
	client.getResponseToSet().setSettings(this->my_config);
	this->set_settings(client, my_config.locations);
	choose_path(client.getResponseToSet(), method.first);
	if (method.second == false || tmp != "HTTP/1.1")
		client.getResponseToSet().error_response(404, this->my_config.error_pages.find(404)->second);
	else if (method.first == "GET" && client.getResponseToSet().getSettings().getMethods() > 3)
		this->manage_get(client);
	else if (method.first == "POST" && (client.getResponseToSet().getSettings().getMethods() == 2
			|| client.getResponseToSet().getSettings().getMethods() == 3 || client.getResponseToSet().getSettings().getMethods() > 5))
		this->manage_post(client);
	else if (method.first == "DELETE" && (client.getResponseToSet().getSettings().getMethods() == 1
			|| client.getResponseToSet().getSettings().getMethods() == 3 || client.getResponseToSet().getSettings().getMethods() == 5
			|| client.getResponseToSet().getSettings().getMethods() == 7))
		this->manage_delete(client);
	else
		client.getResponseToSet().error_response(405, this->my_config.error_pages.find(405)->second);
	client.clearRequest();
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
				if (response.getHeader("Connection") == "close")
					client.setTimeout(-1);
			}
		}
	}
	return (0);
}
