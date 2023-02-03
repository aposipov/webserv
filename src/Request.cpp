/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:40:04 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/02 01:34:40 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() { std::cout << "Default constructor" << std::endl; }

Request::Request(std::string const &request)
{
	std::cout << "Fill the blank" << request << std::endl;
}

Request::Request(Request const &rhs) : requests(rhs.requests)
{
	std::cout << "Copy constructor" << std::endl;
}

Request::~Request()
{
	std::cout << "Destructor of request" << std::endl;
}

Request	&Request::operator=(Request const &rhs)
{
	if (this != &rhs)
		requests = rhs.requests;
	return *this;
}