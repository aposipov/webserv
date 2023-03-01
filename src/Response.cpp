/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:32 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/17 13:52:15 by mnathali         ###   ########.fr       */
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

std::string	Response::getHeader(std::string const &header) const
{
	std::size_t n = this->content.find(header);

	if (n == std::string::npos)
		return "";
	return this->content.substr(this->content.find(" ", n) + 1, this->content.find("\r\n", n));
}

int	Response::fillRedirection()
{
	int code = this->getSettings().getRedirection().first;
	std::string const &direction = this->getSettings().getRedirection().second;

	if (code == 300)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 300 Multiple Choices");
	else if (code == 301)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 301 Moved Permanently");
	else if (code == 302)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 302 Found");
	else if (code == 303)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 303 See Other");
	else if (code == 303)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 304 Not Modified");
	else if (code == 305)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 305 Use Proxy");
	else if (code == 307)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 307 Temporary Redirect");
	else if (code == 308)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 308 Permanent Redirect");
	this->content.append("Location: " + direction + "\r\n");
	this->content.append("\r\n");
	return 0;
}

int	Response::autoindex()
{
	DIR	*catalog = opendir(this->req_path.c_str());
	struct dirent	*dir = 0;
	std::string		tmp;
	std::string		sz;
	std::stringstream ss;

	if (!catalog && this->req_path.size() && this->req_path[this->req_path.size() - 1] != '/')
		catalog = opendir(this->req_path.substr(0, this->req_path.find_last_of("/")).c_str());
	if (!catalog)
		return 1;
	
	tmp = "<!DOCTYPE html>\n<html><big>";
	while ((dir = readdir(catalog)))
	{
		tmp += dir->d_name;
		if (dir->d_type == 4)
			tmp += "/";
		tmp += "<br>";
	}
	tmp += "</big></html>\n";
	ss << tmp.size();
	sz = ss.str();
	this->content.replace(0, this->content.find('\r'), "HTTP/1.1 404 Not Found");
	this->content.append("Content-Type: text/html\r\n");
	this->content.append("Accept-Ranges: bytes\r\n");
	this->content.append("Content-Length: " + sz + "\r\n");
	this->content.append("\r\n");
	this->content.append(tmp);
	closedir(catalog);
	return 0;
}

int	Response::error_response(int code, std::string const &path_to_page)
{
	std::stringstream ss;
	std::string sz;
	std::string error_page;
	std::ifstream	ifs;

	if (path_to_page != "")
		ifs.open(path_to_page.c_str());
	if (!ifs.is_open())
	{
		if (code == 404)
			error_page = "<!DOCTYPE html>\n<html><div id=\"main\"><div class=\"fof\"><h1>Error 404 - Not found</h1></div></div></html>\n";
		else if (code == 400)
			error_page = "<!DOCTYPE html>\n<html><div id=\"main\"><div class=\"fof\"><h1>Error 400 - Bad request</h1></div></div></html>\n";
		else if (code == 405)
			error_page = "<!DOCTYPE html>\n<html><div id=\"main\"><div class=\"fof\"><h1>Error 405 - Method not allowed</h1></div></div></html>\n";
		else if (code == 403)
			error_page = "<!DOCTYPE html>\n<html><div id=\"main\"><div class=\"fof\"><h1>Error 403 - Forbidden</h1></div></div></html>\n";
	}
	else
	{
		std::streampos size;
  		char *memblock;
		size = ifs.tellg();
    	memblock = new char [size];
    	ifs.seekg(0, std::ios::beg);
    	ifs.read(memblock, size);
		ifs.close();
		error_page = memblock;
		delete [] memblock;
	}
	
	if (code == 404)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 404 Not Found");
	else if (code == 400)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 400 Bad Request");
	else if (code == 405)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 405 Method Not Allowed");
	else if (code == 403)
		this->content.replace(0, this->content.find('\r'), "HTTP/1.1 403 Forbidden");

	ss << error_page.size();
	sz = ss.str();
	this->content.append("Content-Type: text/html\r\n");
	this->content.append("Accept-Ranges: bytes\r\n");
	this->content.append("Content-Length: " + sz + "\r\n");
	this->content.append("\r\n");
	this->content.append(error_page);

	return 0;
}