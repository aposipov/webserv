/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:32 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/06 02:47:21 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(std::string response)
{
	headers.push_back(response);
	std::cout << "Fill the blank" << response << std::endl;
}

Response::Response(Response const &rhs) : headers(rhs.headers), content(rhs.content)
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
		headers = rhs.headers;
		content = rhs.content;
	}
	return *this;
}

int	Response::fillHeaders(std::string header)
{
	headers.push_back(header);
	return 0;
}

void	Response::clear()
{
	headers.clear();
	content.clear();
}

std::string	&Response::getContentToFill() { return content; }

std::vector<std::string>	&Response::getHeadersToSet() { return headers; }
