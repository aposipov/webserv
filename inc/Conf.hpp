/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 19:13:53 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/14 00:38:40 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_HPP
#define CONF_HPP

#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <unistd.h>

#include <ctime>


class Server;

class Location
{
	public:

	friend class Server;

	Location();
	Location(Location const &rhs);
	Location(Location const &rhs, std::map<std::string, Location> locs);
	~Location();
	Location &operator=(Location const &rhs);

	int	set_methods(std::vector<std::string> const &which_methods);
	int	set_autoindex(std::vector<std::string> const &which_autoindex);
	int	set_index(std::vector<std::string> const &which_index);
	int	set_root(std::vector<std::string> const &where_root);
	int	set_uploadpath(std::vector<std::string> const &where_path);
	int	set_cgi(std::vector<std::string> const &which_cgi);
	int	set_redirection(std::vector<std::string> const &which_redirection);
	int	set_locations(std::vector<std::string> const &which_locations, std::ifstream &ifs);


	int			getMethods() const;
	bool		getAutoindex() const;
	std::string	getIndex() const;
	std::string	getRoot() const;
	std::string	getUpath() const;
	std::string	getCgi() const;
	std::string	getBin() const;
	std::pair<int, std::string> const &getRedirection() const;

	private:


	int								methods;
	bool							autoindex;
	std::string						index;
	std::string						root;
	std::string						upload_path;
	std::string						cgi_param;
	std::string						bin;
	std::pair<int, std::string>		redirection;
	std::map<std::string, Location>	locations;

};


class Conf : public Location
{

	public:

	friend class Server;

	struct ConfigCreator
	{

		static int	fulfil_conf(std::string const &path, std::vector<Conf> &configs_for_servers);

	};

	typedef std::map<std::string, std::vector<std::string> >	listen_t;
	typedef std::map<int, std::string>							error_t;

	Conf();
	Conf(Conf const &rhs);
	~Conf();
	Conf &operator=(Conf const &rhs);

	int	set_listen(std::vector<std::string> const &what_listen);
	int	set_name(std::vector<std::string> const &which_name);
	int	set_buf_size(std::vector<std::string> const &which_size);
	int	set_keep_time_alive(std::vector<std::string> const &which_time);
	int	set_error_page(std::vector<std::string> const &what_error);

	private:

	listen_t	listen;
	std::string	server_name;
	std::size_t	client_body_buffer_size;
	std::time_t		keep_time_alive;					
	error_t		error_pages;

};


#endif