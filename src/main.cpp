
#include "Server.hpp"

#include <unistd.h>
#include <map>
#include <vector>
#include <fstream>

std::map<std::string, std::vector<std::string> >	create_table(std::string path)
{
	std::ifstream	ifs;
	std::string 	dst;
	std::map<std::string, std::vector<std::string> > config_table;

	ifs.open(path.c_str());
	if (!ifs.is_open())
		throw(std::invalid_argument("File can't be oppened"));
	while (std::getline(ifs, dst))
    {
		std::pair<std::string, std::vector<std::string> > row;
		std::size_t m = dst.find_first_not_of(" \n\f\r\t\v");
		if (m == std::string::npos)
			continue ;
		std::size_t n = dst.find_first_of(" \n\f\r\t\v", m);
		row.first = dst.substr(m, n - m);
		n = dst.find_first_not_of(" \n\f\r\t\v", n);
		while (n != std::string::npos)
		{
			m = dst.find_first_of(" \n\f\r\t\v", n);
			row.second.push_back(dst.substr(n, m - n));
			n = dst.find_first_not_of(" \n\f\r\t\v", m);
		}
		config_table.insert(row);
    }
    ifs.close();
	return (config_table);
}

int		create_server(std::string path)
{

	std::map<std::string, std::vector<std::string> > config_table;
	
	try
	{
		config_table = create_table(path);
		//Здесь можно сделать функцию для проверки содержимого файла конфига
		//и отбросить ненужные строчки из конфига, которые сервер не знает.
		// или если такие строчки есть, то просто сервер не будет запускаеться
		//из-за неправильного файла конфигурации - кинуть эксцепшн, который вернет нас в майн
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		throw ;
	}
	//Далее запуск сервера

	// проверка, как программа прочитала файл
	// for (std::map<std::string, std::vector<std::string> >::iterator it = config_table.begin(); it != config_table.end(); ++it)
	// {
	// 	std::cout << it->first << " -> ";
	// 	for (std::vector<std::string>::iterator ite = it->second.begin(); ite != it->second.end(); ++ite)
	// 		std::cout << *ite << " ";
	// 	std::cout << std::endl;
	// }

	Server server(config_table);
	while (1)
	{
		server.request();
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