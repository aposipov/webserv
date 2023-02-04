/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 00:25:00 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/04 02:27:26 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Conf.hpp"

Location::Location() : methods(7), autoindex(1), index(""), root("/"), upload_path("/"), cgi_param("")//, error_pages()
{
	std::cout << "------>Location init<------" << std::endl;
}

Location::Location(Location const &rhs) : methods(rhs.methods), autoindex(rhs.autoindex), index(rhs.index), root(rhs.root), upload_path(rhs.upload_path),
	cgi_param(rhs.cgi_param)//, error_pages(rhs.error_pages)
{
	std::cout << "------>Location init<------" << std::endl;
}

Location::~Location()
{
	std::cout << "------>conf deleted<------" << std::endl;
}

Location &Location::operator=(Location const &rhs)
{
	if (this != &rhs)
	{
		methods = rhs.methods;
		autoindex = rhs.autoindex;
		index = rhs.index;
		root = rhs.root;
		upload_path = rhs.upload_path;
		cgi_param = rhs.cgi_param;
		bin = rhs.bin;
	}
	return *this;
}

Conf::Conf() : Location(), listen(), server_name("localhost"),  client_body_buffer_size(4000), locations()
{
	std::cout << "------>conf init<------" << std::endl;
}

Conf::~Conf()
{
	std::cout << "------>conf deleted<------" << std::endl;
}

Conf::Conf(Conf const &rhs) : Location(rhs), listen(rhs.listen), server_name(rhs.server_name),
	client_body_buffer_size(rhs.client_body_buffer_size), locations(rhs.locations)
{
	std::cout << "------>conf ini<------" << std::endl;
}

int	Location::getMethods() const { return methods; }
bool	Location::getAutoindex() const { return autoindex; }
std::string	Location::getIndex() const { return index; }
std::string	Location::getRoot() const { return root; }
std::string	Location::getUpath() const { return upload_path; }
std::string	Location::getCgi() const { return cgi_param; }
std::string	Location::getBin() const { return bin; }


Conf &Conf::operator=(Conf const &rhs)
{
	if (this != &rhs)
	{
		Location &tmp = *this;
		tmp = static_cast<Location const &>(rhs);
		listen = rhs.listen;
		server_name = rhs.server_name;
		client_body_buffer_size = rhs.client_body_buffer_size;
		locations = rhs.locations;
	}
	return *this;
}

int	Conf::set_listen(std::vector<std::string> const &what_listen)
{
	for (std::vector<std::string>::const_iterator it = what_listen.begin(); it < what_listen.end(); ++it)
	{
		std::pair<std::string, std::vector<std::string> > row;
		std::size_t pos = it->find_first_of(':');
		if (pos == std::string::npos && it->find_first_not_of("0123456789;") == std::string::npos)
		{
			row.first = "0.0.0.0";
			if (*it != ";")
				listen.insert(row).first->second.push_back(*it);
		}
		else if (pos == it->find_last_of(':') && pos != std::string::npos && pos != it->size() - 1)
		{
			row.first = it->substr(0, it->find_first_of(':'));
			if (row.first.find_first_not_of("0123456789.abcdef") != std::string::npos)
				throw(std::invalid_argument("Invalid configuration file: wrong adress " + row.first));
			listen.insert(row).first->second.push_back(it->substr(it->find_first_of(':') + 1));
		}
		else
			throw(std::invalid_argument("Invalid configuration file: wrong adress " + *it));
	}
	return 0;
}

int	Conf::set_name(std::vector<std::string> const &which_name)
{
	if ((which_name.size() == 1 && !which_name.front().empty()) || (which_name.size() == 2 && which_name.back() == ";"))
		server_name = which_name.front();
	else
		throw(std::invalid_argument("Invalid configuration file: wrong name"));
	if (server_name.back() == ';')
		server_name.erase(server_name.size() - 1);
	return 0;
}

int	Location::set_methods(std::vector<std::string> const &which_methods)
{
	methods = 0;
	for (std::vector<std::string>::const_iterator it = which_methods.begin(); it < which_methods.end(); ++it)
	{
		if (it->find("GET") != std::string::npos && methods < 4)
			methods += 4;
		if (it->find("POST") != std::string::npos && methods < 6)
			methods += 2;
		if (it->find("DELETE") != std::string::npos && methods < 7)
			methods += 1;
		if (methods > 7)
			throw(std::invalid_argument("Invalid configuration file: wrong methods"));
	}
	return 0;
}

int	Location::set_autoindex(std::vector<std::string> const &which_autoindex)
{
	if ((which_autoindex.front() == "on" || which_autoindex.front() == "on;") && (which_autoindex.size() == 1 || which_autoindex.size() == 2))
		autoindex = true;
	else if ((which_autoindex.front() == "off" || which_autoindex.front() == "off;") && (which_autoindex.size() == 1 || which_autoindex.size() == 2))
		autoindex = false;
	else
		throw(std::invalid_argument("Invalid configuration file: wrong autoindex"));
	return 0;
}

int	Location::set_index(std::vector<std::string> const &which_index)
{
	if (which_index.size() == 1 || (which_index.size() == 2 && which_index.back() == ";"))
		index = which_index.front();
	else
		throw(std::invalid_argument("Invalid configuration file: wrong index"));
	if (index.back() == ';')
		index.erase(index.size() - 1);
	std::cout << this->index << std::endl;
	return 0;
}

int	Location::set_root(std::vector<std::string> const &where_root)
{
	if (where_root.size() == 1 || (where_root.size() == 2 && where_root.back() == ";"))
		root = where_root.front();
	else
		throw(std::invalid_argument("Invalid configuration file: wrong root"));
	if (root.back() == ';')
		root.erase(root.size() - 1);
	return 0;
}

int	Location::set_uploadpath(std::vector<std::string> const &where_path)
{
	if (where_path.size() == 1 || (where_path.size() == 2 && where_path.back() == ";"))
		upload_path = where_path.front();
	else
		throw(std::invalid_argument("Invalid configuration file: wrong path"));
	if (upload_path.back() == ';')
		upload_path.erase(upload_path.size() - 1);
	return 0;
}

int	Conf::set_locations(std::vector<std::string> const &which_locations, std::ifstream &ifs)
{
	std::string		dst;
	unsigned short	bkt = 0;

	if (which_locations.empty() || which_locations.size() > 2 || (which_locations.size() == 2 && which_locations.back() != "{"))
		throw(std::invalid_argument("Invalid configuration file: wrong location"));
	if (which_locations.back() == "{" || (which_locations.size() == 1 && which_locations.back().back() == '{'))
		++bkt;
	std::pair<std::string, Location>	tmp(which_locations.front(), Location());
	if (which_locations.size() == 1 && which_locations.back().back() == '{')
		tmp.first.erase(tmp.first.size() - 1);
	Location &loc = locations.insert(tmp).first->second;
	while (std::getline(ifs, dst))
    {
		std::pair<std::string, std::vector<std::string> > row;
		std::size_t m = dst.find_first_not_of(" \n\f\r\t\v");
		if (m == std::string::npos)
			continue ;
		std::size_t n = dst.find_first_of(" \n\f\r\t\v", m);
		row.first = dst.substr(m, n - m);
		n = dst.find_first_not_of(" \n\f\r\t\v", n);
		while (n != std::string::npos)
		{
			m = dst.find_first_of(" \n\f\r\t\v", n);
			row.second.push_back(dst.substr(n, m - n));
			n = dst.find_first_not_of(" \n\f\r\t\v", m);
		}
		if (row.first == "{" && row.second.empty())
			++bkt;
		else if ((row.first == "}" && row.second.empty()) || (!row.second.empty() && row.second.back().back() == '}'))
			--bkt;
		else if (row.first == "{" || row.first == "}")
			throw(std::invalid_argument("Invalid configuration file: brackets { } should be followed by new line"));
		else if (row.first == "methods")
			loc.set_methods(row.second);
		else if (row.first == "autoindex")
			loc.set_autoindex(row.second);
		else if (row.first == "index")
			loc.set_index(row.second);
		else if (row.first == "root")
			loc.set_root(row.second);
		else if (row.first == "upload_path")
			loc.set_uploadpath(row.second);
		else if (row.first == "cgi_param")
			loc.set_cgi(row.second);
		else
			throw(std::invalid_argument("Invalid configuration file: unknown rule " + row.first));
		if (bkt == 0)
			break ;
    }
	return 0;
}

int	Conf::set_buf_size(std::vector<std::string> const &which_size)
{
	if (which_size.size() == 1 || (which_size.size() == 2 && which_size.back() == ";"))
		client_body_buffer_size = atoi(which_size.front().c_str());
	else
		throw(std::invalid_argument("Invalid configuration file: wrong buffer size"));
	if (client_body_buffer_size > 4096 || client_body_buffer_size < 1)
		client_body_buffer_size = 256;
	return 0;
}

int	Location::set_cgi(std::vector<std::string> const &which_cgi)
{
	if (which_cgi.size() == 1 || (which_cgi.size() == 2 && which_cgi.back() == ";"))
		cgi_param = which_cgi.front();
	else
		throw(std::invalid_argument("Invalid configuration file: wrong cgi"));
	if (cgi_param.back() == ';')
		cgi_param.erase(cgi_param.size() - 1);
	return 0;
}


int	Conf::ConfigCreator::fulfil_conf(std::string const &path, std::vector<Conf> &configs_for_servers)
{
	std::ifstream	ifs;
	std::string 	dst;
	unsigned short	bkt = 0;

	ifs.open(path);
	if (!ifs.is_open())
		throw(std::invalid_argument("File can't be oppened"));
	while (std::getline(ifs, dst))
    {
		std::pair<std::string, std::vector<std::string> > row;
		std::size_t m = dst.find_first_not_of(" \n\f\r\t\v");
		if (m == std::string::npos)
			continue ;
		std::size_t n = dst.find_first_of(" \n\f\r\t\v", m);
		row.first = dst.substr(m, n - m);
		if (!row.first.find('#'))
			continue ;
		n = dst.find_first_not_of(" \n\f\r\t\v", n);
		while (n != std::string::npos)
		{
			m = dst.find_first_of(" \n\f\r\t\v", n);
			row.second.push_back(dst.substr(n, m - n));
			n = dst.find_first_not_of(" \n\f\r\t\v", m);
		}
		if (row.first == "server" || row.first == "server{")
		{
			if (bkt != 0)
				return (-1);
			configs_for_servers.push_back(Conf());
			if (row.first == "server{" || (!row.second.empty() && row.second[0] == "{"))
				++bkt;
			if (row.second.size() > 1)
				throw(std::invalid_argument("Invalid configuration file: brackets { } should be followed by new line"));
		}
		else if (row.first == "{" && row.second.empty())
			++bkt;
		else if ((row.first == "}" && row.second.empty()) || (!row.second.empty() && row.second.back().back() == '}'))
			--bkt;
		else if (row.first == "{" || row.first == "}")
			throw(std::invalid_argument("Invalid configuration file: brackets { } should be followed by new line"));
		else if (row.first == "listen")
			configs_for_servers.back().set_listen(row.second);
		else if (row.first == "server_name")
			configs_for_servers.back().set_name(row.second);
		else if (row.first == "methods")
			configs_for_servers.back().set_methods(row.second);
		else if (row.first == "autoindex")
			configs_for_servers.back().set_autoindex(row.second);
		else if (row.first == "index")
			configs_for_servers.back().set_index(row.second);
		else if (row.first == "root")
			configs_for_servers.back().set_root(row.second);
		else if (row.first == "upload_path")
			configs_for_servers.back().set_uploadpath(row.second);
		else if (row.first == "location")
			configs_for_servers.back().set_locations(row.second, ifs);
		else if (row.first == "client_body_buffer_size")
			configs_for_servers.back().set_buf_size(row.second);
		else if (row.first == "cgi_param")
			configs_for_servers.back().set_cgi(row.second);
		else
			throw(std::invalid_argument("Invalid configuration file: unknown rule " + row.first));
    }
    ifs.close();
	if (bkt != 0 || !ifs.eof())
		return (-1);
	return (0);

}