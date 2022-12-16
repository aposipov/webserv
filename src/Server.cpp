#include "../inc/Server.hpp"

Server::Server(std::map<std::string, std::vector<std::string> > &config_table) : listen_fd(-1), connfd(-1), pid(1)
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
	if ((listen(listen_fd, SOMAXCONN)) < 0)
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

	connfd = accept(listen_fd, NULL, NULL);
	//Может здесь сделать fork, чтобы пустить обработку запроса в отдельный процесс,
	//а основной процесс вернется в цикл
	// std::cout << "lalal" << connfd << std::endl;
	if ((pid = fork()) < 0)
		throw(std::logic_error("Fork error"));
	if (pid != 0)
		close(connfd);
	return (0);
}

int Server::get_my_pid() const { return pid; }

int	Server::manage_request()
{
	int		n;
	char	buf[SIZE_OF_BUF];
	std::string	Buf;

	memset(buf, 0, SIZE_OF_BUF);
	while ((n = recv(connfd, buf, SIZE_OF_BUF - 1, 0)) > 0)
	{
		Buf.append(buf, n);
		if (Buf.substr(Buf.size() - 4) == "\r\n\r\n")
			break ;
	}
	if (n < 0)
		std::cout << "Read error" << std::endl;
	std::cout << Buf << std::endl;
	std::string response = "HTTP/1.1 200 OK\r\n\r\n";
	//parse Header

	//send file
	std::ifstream ifs;
	ifs.open(Buf.substr(Buf.find('/')+1, Buf.find(' ', Buf.find('/')) - Buf.find('/')-1));
	if (ifs.is_open())
	{
		std::string dst;
		while (std::getline(ifs, dst))//Для передачи файла, можно поставить std::binary mode и читать через ifs.read()
		{
			response.append(dst);
		}
		std::cout << response;
		ifs.close();
	}
	else
		response += "<!DOCTYPE html><html><head><title>Example</title></head><body><p>This is an example of a simple HTML page with one paragraph.</p></body></html>";
	send(connfd, response.c_str(), response.size(), 0);
	close(connfd);
	return (0);
}



