/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:27 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/08 14:46:44 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include "Conf.hpp"

class Response
{

	public:

	Response(std::string response = "HTTP/1.1 200 OK\r\n");
	Response(Response const &rhs);
	~Response();
	Response	&operator=(Response const &rhs);
	
	std::string	&getContentToFill();
	int		setSendSize(ssize_t	sz);
	ssize_t	getSendedSize() const;

	int	error_response(int code = 404);

	private:

	std::string	content;
	ssize_t		sended_size;
	Location	actual_settings;
	
};

#endif