/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:40:04 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/05 02:13:10 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() { std::cout << "Default constructor request" << std::endl; }

Request::Request(std::string const &request)
{
	std::cout << "Fill the blanks" << std::endl;
	std::pair<std::string, std::string>	row;
	std::size_t	m = request.find("\r\n");
	std::string bottom = request.substr(0, m);
	std::size_t	n = bottom.rfind(' ');
	std::size_t l = request.find(' ');

	if (n != std::string::npos && l != std::string::npos)
	{
		row.first = "Method"; row.second = request.substr(0, request.find(' '));
		headers.insert(row);
		row.first = "Protocol"; row.second = bottom.substr(n + 1, m - n - 1);
		headers.insert(row);
		row.first = "Path"; row.second = bottom.substr(request.find(' ') + 1, n - l - 1);
		headers.insert(row);
	}
	std::cout << "---------------------------------\n";
	while (m < request.size() && request.substr(m, 4) != "\r\n\r\n")
	{
		n = m + 2; m = request.find("\r\n", n);
		l = request.find(' ', n);
		row.first = request.substr(n, l - n);
		row.first.erase(row.first.size() - 1);
		row.second = request.substr(l + 1, m - l - 1);
		headers.insert(row);
		std::cout << n << " " << request.size() << std::endl;
	}
	if (request.size() - m > 3)
		content = request.substr(m + 3);
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		std::cout << it->first << " ::----:: " << it->second << std::endl;
}

Request::Request(Request const &rhs) : headers(rhs.headers), content(rhs.content)
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
		headers = rhs.headers;
		content = rhs.content;
	}
	return *this;
}

std::pair<std::string, bool>	Request::getHeader(std::string const &header) const
{
	std::map<std::string, std::string>::const_iterator it = headers.find(header);
	if (it == headers.end())
		return std::pair<std::string, bool>("", false);
	return std::pair<std::string, bool>(it->second, true);
}

void	Request::clear()
{

	std::map<std::string, std::string> replacement;
	std::string empty_string;

	headers.swap(replacement);
	content.swap(empty_string);
}
