/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:40:04 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/12 16:51:03 by mnathali         ###   ########.fr       */
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
		row.first = "REQUEST_METHOD"; row.second = request.substr(0, request.find(' '));
		headers.insert(row);
		row.first = "SERVER_PROTOCOL"; row.second = bottom.substr(n + 1, m - n - 1);
		headers.insert(row);
		row.first = "Path"; row.second = bottom.substr(request.find(' ') + 1, n - l - 1);
		headers.insert(row);
	}
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
	this->query_string();
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

int	Request::query_string()
{
	std::pair<std::string, std::string> row("QUERY_STRING", "");
	std::string &path = this->headers["Path"];
	std::string::size_type	index = path.find('?');
	if (index != std::string::npos)
	{
		row.second = path.substr(index + 1);
		path.erase(index);
	}
	headers.insert(row);
	return 0;
}


std::pair<std::string, bool>	Request::getHeader(std::string const &header) const
{
	std::map<std::string, std::string>::const_iterator it = headers.find(header);
	if (it == headers.end())
		return std::pair<std::string, bool>("", false);
	return std::pair<std::string, bool>(it->second, true);
}

std::map<std::string, std::string>::const_iterator Request::headers_begin() const
{
	return this->headers.begin();
}

std::map<std::string, std::string>::const_iterator Request::headers_end() const
{
	return this->headers.end();
}


std::string	const &Request::getContent() const { return this->content; }

void	Request::clear()
{

	headers.clear();
	content.clear();
}
