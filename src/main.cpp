#include "Server.hpp"


#include "Conf.hpp"


int	create_and_launch_server(std::string path)
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
	std::multimap<int, Server*> serv_base_fds;
	std::map<int, Server*> serv_child_fds;
	std::map<std::string, Server*>	serv_by_name;
	fd_set	who_read, who_write, copy;
	int max_fd = 0;
	FD_ZERO(&copy);

	try
	{
		for (std::vector<Conf>::iterator it = configs_for_servers.begin(); it < configs_for_servers.end(); ++it)
		{
			servers.push_back(Server(*it));
			if (serv_by_name.find(servers.back().get_my_name()) == serv_by_name.end())
				serv_by_name.insert(std::pair<std::string, Server*>(servers.back().get_my_name(), &servers.back()));
			for (std::vector<int>::const_iterator ite = servers.back().get_listen_fd().begin(); ite < servers.back().get_listen_fd().end(); ++ite)
			{
				serv_base_fds.insert(std::pair<int, Server*>(*ite, &servers.back()));
				if (*ite > max_fd)
					max_fd = *ite;
				FD_SET(*ite, &copy);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		throw;
	}
	
	while (1)
	{
		who_read = who_write = copy;
		int sel = select(max_fd + 1, &who_read, &who_write, NULL, NULL);
		std::cout << "Select = " << sel << "; found action on write or read set" << std::endl;
		for (int i = 0; i <= max_fd; ++i)
			std::cout << i << " Is_set_read " << FD_ISSET(i, &who_read) << " | Is_set_write " << FD_ISSET(i, &who_write) << std::endl;
		// sleep(2);
		for (int i = 0; i <= max_fd; ++i)
		{
			if (!FD_ISSET(i, &who_read) && !FD_ISSET(i, &who_write) && serv_child_fds.find(i) == serv_child_fds.end())
				continue;
			if (serv_base_fds.find(i) != serv_base_fds.end())
			{
				Server *acceptin_server = serv_base_fds.find(i)->second;
				int connfd = acceptin_server->add_new_client(i);
				if (connfd <= 0)
					continue ;
				serv_child_fds.insert(std::pair<int, Server*>(connfd, acceptin_server));
				FD_SET(connfd, &copy);
				if (max_fd < connfd)
					max_fd = connfd;
				std::cout << "New max_fd = " << max_fd << std::endl;break;
			}
			else if (FD_ISSET(i, &who_read))
			{
				Server *procc_serv = serv_child_fds.at(i);
				int	cont_rem = procc_serv->get_request(i);
				if (cont_rem > 0)
					procc_serv->manage_request(i);
			}
			else if (FD_ISSET(i, &who_write))
				serv_child_fds.at(i)->action_response(i);
			if (serv_base_fds.find(i) == serv_base_fds.end() && serv_child_fds.at(i)->check_timeout(i))
			{
				serv_child_fds.at(i)->erase_client(i);
				serv_child_fds.erase(i);
				FD_CLR(i, &copy);
				while (!FD_ISSET(max_fd, &copy))
					--max_fd;
			}
		}
	}

	return (0);
}

int main(int ac, char **av)
{
	std::cout << *av << std::endl;

	std::cout << "Hello webserv!" << std::endl;
	if (ac > 2)
		std::cerr << "Error: too many arguments" << std::endl;
	else
	{
		try
		{
			if (ac == 2)
				create_and_launch_server(av[1]);
			else
				create_and_launch_server("./config/test.conf");

		}
		catch(const std::exception& e)
		{
			std::cerr << "Server wasn't launched" << std::endl;
		}
	}
	return(0);
}
