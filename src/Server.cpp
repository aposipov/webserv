#include "../inc/Server.hpp"

Server::Server(std::map<std::string, std::vector<std::string> > &config_table)
{
	this->config_table.swap(config_table);
	std::cout << "Server is created" << std::endl;
	if ((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		std::cerr << "Socket error" << std::endl;//exception
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	if ((bind(listen_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
		std::cerr << "Bind error" << std::endl;//exception
	if ((listen(listen_fd, 10)) < 0)
		std::cerr << "Listen error" << std::endl;//exception
	std::cout << "Server uses socket " << listen_fd << " and it accepts connections ";
	std::cout << servaddr.sin_addr.s_addr << " port " << servaddr.sin_port;
	std::cout << " and protocol is " << servaddr.sin_family << std::endl;
}

Server::~Server()
{
	std::cout << "Server finished" << std::endl;
}

int	Server::request()
{
	int connfd, n;
	char buf[4096];

	memset(buf, 0, 4096);
	connfd = accept(listen_fd, NULL, NULL);
	//Может здесь сделать fork, чтобы пустить обработку запроса в отдельный процесс,
	//а основной процесс вернется в цикл
	// std::cout << "lalal" << connfd << std::endl;
	while ((n = read(connfd, buf, 4096 - 1)) > 0)
	{
		std::cout << buf;
		if (std::string(&buf[n-4]) == "\r\n\r\n")
			break ;
	}
	if (n < 0)
		std::cout << "Read error" << std::endl;
	std::string response = "HTTP/1.1 200 OK\r\n\r\n<!DOCTYPE html><html><head><title>Example</title></head><body><p>This is an example of a simple HTML page with one paragraph.</p></body></html>";
	write(connfd, response.c_str(), response.size());
	close(connfd);
	return (0);
}


