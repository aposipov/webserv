/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:32 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/10 10:10:46 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(std::string response) : content(response), sended_size(0),
	actual_settings(), req_path("")
{
	std::cout << "Fill the blank" << response << std::endl;
}

Response::Response(Response const &rhs) : content(rhs.content), sended_size(rhs.sended_size),
	actual_settings(rhs.actual_settings), req_path(rhs.req_path)
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
		req_path = rhs.req_path;
	}
	return *this;
}


std::string	&Response::getContentToFill() { return this->content; }

int		Response::setSendSize(ssize_t	sz)
{
	sended_size = sz;
	return 0;
}

const Location	&Response::getSettings() const { return this->actual_settings; }

int	Response::setSettings(Location const &loc)
{
	this->actual_settings = Location(loc, std::map<std::string, Location>());
	return 0;
}

ssize_t		Response::getSendedSize() const { return this->sended_size; }

std::string	Response::getPath() const { return this->req_path; }

int	Response::setPath(std::string const &path)
{
	this->req_path = path;
	return 0;
}

int	Response::autoindex()
{
	
	return 0;
}


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