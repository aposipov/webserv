/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:43:38 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/05 00:49:34 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int const my_fd) : my_fd(my_fd)
{
	bzero(&client_addr, sizeof(client_addr));
}

Client::~Client() { std::cout << "Client destructor" << std::endl; }

Client::Client(Client const &rhs) : my_fd(rhs.my_fd), client_addr(rhs.client_addr),
	my_message(rhs.my_message), request(rhs.request), response(rhs.response)
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
		request = rhs.request;
		response = rhs.response;
	}
	return *this;
}

struct sockaddr *Client::getClientAddr() { return &client_addr; }

int	Client::get_myFd() const { return my_fd; }

std::string	&Client::messageRef() { return my_message; }

int	Client::set_myFd(int const value)
{
	my_fd = value;
	return 0;
}

int	Client::setRequest(Request const &req)
{
	request = req;
	return (0);
}

Response	&Client::getResponseToSet() { return response; }


const Request &Client::getReqest() const { return request; }

void	Client::clearRequest() { request.clear(); }
void	Client::clearResponse() { response.clear(); }
