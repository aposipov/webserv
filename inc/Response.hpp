/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:27 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/05 04:02:05 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <vector>

class Response
{

	public:

	Response(std::string response = "HTTP/1.1 200 OK");
	Response(Response const &rhs);
	~Response();
	Response	&operator=(Response const &rhs);
	
	int			fillHeaders(std::string header);
	std::vector<std::string>	&getHeadersToSet();
	std::string	&getContentToFill();
	void		clear();

	private:

	std::vector<std::string>	headers;
	std::string					content;

};

#endif