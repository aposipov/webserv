/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:40:04 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/04 01:34:46 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() { std::cout << "Default constructor request" << std::endl; }

Request::Request(std::string const &request)
{
	std::cout << "Fill the blanks" << std::endl;
	std::pair<std::string, std::string>	row;
	std::size_t	m = request.find('\n');
	std::string bottom = request.substr(0, m);
	std::size_t	n = bottom.rfind(' ');
	std::size_t l = request.find(' ');
	row.first = "Method"; row.second = request.substr(0, request.find(' '));
	requests.insert(row);
	row.first = "Protocol"; row.second = bottom.substr(n + 1, m - n - 1);
	requests.insert(row);
	row.first = "Path"; row.second = bottom.substr(request.find(' ') + 1, n - l - 1);
	requests.insert(row);
	std::cout << "---------------------------------\n";
	while (m < request.size() && request.substr(m - 1, 4) != "\r\n\r\n")
	{
		n = m + 1; m = request.find('\n', n);
		l = request.find(' ', n);
		row.first = request.substr(n, l - n);
		row.first.erase(row.first.size() - 1);
		row.second = request.substr(l + 1, m - l - 1);
		requests.insert(row);
	}
	if (request.size() - m > 3)
		content = request.substr(m + 3);
	for (std::map<std::string, std::string>::iterator it = requests.begin(); it != requests.end(); ++it)
		std::cout << it->first << " ::----:: " << it->second << std::endl;
}

Request::Request(Request const &rhs) : requests(rhs.requests), content(rhs.content)
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
	{
		requests = rhs.requests;
		content = rhs.content;
	}
	return *this;
}
