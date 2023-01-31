/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:43:38 by mnathali          #+#    #+#             */
/*   Updated: 2023/01/31 12:42:43 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int const my_fd) : my_fd(my_fd)
{
	bzero(&client_addr, sizeof(client_addr));
}

Client::~Client() { std::cout << "Client destructor" << std::endl; }

Client::Client(Client const &rhs) : my_fd(rhs.my_fd), client_addr(rhs.client_addr), my_message(rhs.my_message)
{
	std::cout << "Client copied" << std::endl;
}

Client	&Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		my_fd = rhs.my_fd;
		client_addr = rhs.client_addr;
		my_message = rhs.my_message;
	}
	return *this;
}

struct sockaddr *Client::getClientAddr() { return &client_addr; }

std::string	&Client::changeMassage() { return my_message; }

int	Client::get_myFd() const { return my_fd; }

int	Client::set_myFd(int const value)
{
	my_fd = value;
	return 0;
}