/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:35:42 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/11 14:21:51 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include <netinet/in.h>

#include "Request.hpp"
#include "Response.hpp"

#include <strings.h>

class Client
{

	public:

	typedef	struct sockaddr	client_t;

	Client(int const my_fd = -1);
	~Client();
	Client(Client const &rhs);
	Client	&operator=(Client const &rhs);

	struct sockaddr	*getClientAddr();
	int				get_myFd() const;
	int				set_myFd(int const value);
	const Request	&getReqest() const;
	std::string		&messageRef();
	int				setRequest(Request const &req);
	Response		&getResponseToSet();
	void			clearRequest();
	int				setTimeout(std::time_t = 60);
	ssize_t			getTimeout() const;

	private:

	int					my_fd;
	struct sockaddr		client_addr;
	std::string			my_message;
	Request				request;
	Response			response;
	long				my_timeout;

};

#endif