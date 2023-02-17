/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:27 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/14 15:21:12 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include "Conf.hpp"

#include <sys/types.h>
#include <dirent.h>

class Response
{

	public:

	Response(std::string response = "");
	Response(Response const &rhs);
	~Response();
	Response	&operator=(Response const &rhs);
	
	std::string	&getContentToFill();
	int		setSendSize(ssize_t	sz);
	ssize_t	getSendedSize() const;
	std::string	getPath() const;
	const Location	&getSettings() const;
	int		setPath(std::string const &path);
	int		setSettings(Location const &loc);

	std::string	getHeader(std::string const &header) const;

	int	error_response(int code, std::string const &path_to_page);
	int	autoindex();
	int	fillRedirection();

	private:

	std::string	content;
	ssize_t		sended_size;
	Location	actual_settings;
	std::string	req_path;
	
};

#endif