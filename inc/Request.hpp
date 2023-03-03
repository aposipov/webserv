/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:34:33 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/12 16:50:47 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <unistd.h>

class Request
{

	public:

	Request();
	Request(std::string const &request);
	Request(Request const &rhs);
	~Request();
	Request	&operator=(Request const &rhs);

	std::pair<std::string, bool>	getHeader(std::string const &header) const;

	std::string	const &getContent() const;

	void	clear();

	std::map<std::string, std::string>::const_iterator headers_begin() const;
	std::map<std::string, std::string>::const_iterator headers_end() const;

	private:

	int	query_string();

	std::map<std::string, std::string>	headers;
	std::string							content;

};

#endif