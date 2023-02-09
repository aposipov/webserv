/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:32 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/08 14:46:37 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(std::string response) : content(response), sended_size(0)
{
	std::cout << "Fill the blank" << response << std::endl;
}

Response::Response(Response const &rhs) : content(rhs.content), sended_size(rhs.sended_size), actual_settings(rhs.actual_settings)
{
	std::cout << "Copy constructor" << std::endl;
}

Response::~Response()
{
	std::cout << "Destructor of Response" << std::endl;
}

Response	&Response::operator=(Response const &rhs)
{
	if (this != &rhs)
	{
		content = rhs.content;
		sended_size = rhs.sended_size;
		actual_settings = rhs.actual_settings;
	}
	return *this;
}


std::string	&Response::getContentToFill() { return content; }

int		Response::setSendSize(ssize_t	sz)
{
	sended_size = sz;
	return 0;
}

ssize_t		Response::getSendedSize() const { return sended_size; }

int	Response::error_response(int code)
{
	std::stringstream ss;
	std::string sz;
	std::string error_page;

	if (code == 404)
	{
		error_page = "<!DOCTYPE html>\n<html><div id=\"main\"><div class=\"fof\"><h1>Error 404 - Not found</h1></div></div></html>\n";
		ss << error_page.size();
		sz = ss.str();
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 404 Not Found");
		this->content.append("Content-Type: text/html\r\n");
		this->content.append("Accept-Ranges: bytes\r\n");
		this->content.append("Content-Length: " + sz + "\r\n");
		this->content.append("\r\n");
		this->content.append(error_page);
	}
	else if (code == 400)
	{
		error_page = "<!DOCTYPE html>\n<html><div id=\"main\"><div class=\"fof\"><h1>Error 400 - Bad request</h1></div></div></html>\n";
		ss << this->content.size();
		sz = ss.str();
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 404 Not Found");
		this->content.append("Content-Type: text/html\r\n");
		this->content.append("Accept-Ranges: bytes\r\n");
		this->content.append("Content-Length: " + sz + "\r\n");
		this->content.append("\r\n");
		this->content.append(error_page);
	}

	return 0;
}