#include "Server.hpp"

#include "Conf.hpp"

#include <unistd.h>

#include <map>
#include <vector>


int	create_server(std::string path)
{

	std::vector<Conf>	configs_for_servers;

	try
	{
		if (Conf::ConfigCreator::fulfil_conf(path, configs_for_servers) == -1)
			throw(std::invalid_argument("Invalid configuration file: problem with brackets { } or unknown setting"));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		throw;
	}
	std::cout << "Here" << std::endl;

	//Далее запуск сервера

	std::vector<Server>	servers;
	std::map<int, Server&> serv_base_fds;
	std::map<int, Server&> serv_child_fds;
	fd_set	who_read, who_write, copy;
	int max_fd = 0;
	FD_ZERO(&copy);
	// FD_SET(0, &copy);

	try
	{
		for (std::vector<Conf>::iterator it = configs_for_servers.begin(); it < configs_for_servers.end(); ++it)
		{
			servers.push_back(Server(*it));
			for (std::vector<int>::const_iterator ite = servers.back().get_listen_fd().begin(); ite < servers.back().get_listen_fd().end(); ++ite)
			{
				serv_base_fds.insert(std::pair<int, Server&>(*ite, servers.back()));
				if (*ite > max_fd)
					max_fd = *ite;
				FD_SET(*ite, &copy);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return -1;
	}
	
	while (1)
	{
		FD_COPY(&copy, &who_read);
		FD_COPY(&copy, &who_write);
		int sel = select(max_fd + 1, &who_read, &who_write, NULL, NULL);
		std::cout << "Select = " << sel << "; found action on write or read set" << std::endl;sleep(2);
		for (int i = 0; i <= max_fd; ++i)
		{
			if (!FD_ISSET(i, &who_read))
				continue;
			if (serv_base_fds.find(i) != serv_base_fds.end())
			{
				Server &acceptin_server = serv_base_fds.at(i);
				int connfd = acceptin_server.add_new_client(i);
				if (connfd <= 0)
					continue ;
				serv_child_fds.insert(std::pair<int, Server&>(connfd, acceptin_server));
				FD_SET(connfd, &copy);
				if (max_fd < connfd)
					max_fd = connfd;
			}
			else
			{
				int	cont_rem = serv_child_fds.at(i).get_request(i);
				if (cont_rem <= 0)
				{
					if (cont_rem < 0)
					{
						close(i);
						FD_CLR(i, &copy); //  close connection
					}
					continue;
				}
				serv_child_fds.at(i).manage_request(i);
				serv_child_fds.at(i).action_response(i);
			}
		}
	}

	return (0);
}

int main(int ac, char **av)
{
	std::cout << *av << std::endl;

	std::cout << "Hello webserv!" << std::endl;
	if (ac != 2 || access(av[1], F_OK | R_OK))
		std::cerr << "Error: ./webserv <config_file>" << std::endl;
	else
	{
		try
		{
			create_server(av[1]);
		}
		catch(const std::exception& e)
		{
			std::cerr << "Server wasn't launched" << std::endl;
		}
	}
	return(0);
}









// std::map<std::string, std::vector<std::string> >	create_table(std::string path)
// {
// 	std::ifstream	ifs;
// 	std::string 	dst;
// 	std::map<std::string, std::vector<std::string> > config_table;

// 	ifs.open(path);
// 	if (!ifs.is_open())
// 		throw(std::invalid_argument("File can't be oppened"));
// 	while (std::getline(ifs, dst))
//     {
// 		std::pair<std::string, std::vector<std::string> > row;
// 		std::size_t m = dst.find_first_not_of(" \n\f\r\t\v");
// 		if (m == std::string::npos)
// 			continue ;
// 		std::size_t n = dst.find_first_of(" \n\f\r\t\v", m);
// 		row.first = dst.substr(m, n - m);
// 		std::map<std::string, std::vector<std::string> >::iterator key = config_table.insert(row).first;
// 		n = dst.find_first_not_of(" \n\f\r\t\v", n);
// 		while (n != std::string::npos)
// 		{
// 			m = dst.find_first_of(" \n\f\r\t\v", n);
// 			key->second.push_back(dst.substr(n, m - n));
// 			n = dst.find_first_not_of(" \n\f\r\t\v", m);
// 		}
//     }
//     ifs.close();
// 	return (config_table);
// }
