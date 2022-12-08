
#include "server.hpp"

int main(int ac, char **av)
{
	std::cout << *av << std::endl;

	std::cout << "Hello webserv!" << std::endl;
	if (ac != 2)
		std::cerr << "Error: ./webserv <config_file>" << std::endl;
//	else
//		func(av[1]);

}