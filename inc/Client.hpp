/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:35:42 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/02 09:17:22 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include <netinet/in.h>

#include "Request.hpp"
#include "Response.hpp"


class Client
{

	public:

	typedef	struct sockaddr	client_t;

	Client(int const my_fd = -1);
	~Client();
	Client(Client const &rhs);
	Client	&operator=(Client const &rhs);

	struct sockaddr *getClientAddr();
	int	get_myFd() const;
	int	set_myFd(int const value);
	std::string	&messageRef();
	int	setRequest(Request const &req);
	int setResponse(Response const &res);

	private:

	int					my_fd;
	struct sockaddr		client_addr;
	std::string			my_message;
	Request				request;
	Response			response;



};

#endif