/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:32 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/03 02:10:06 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() { std::cout << "Default constructor" << std::endl; }

Response::Response(std::string const &response)
{
	std::cout << "Fill the blank" << response << std::endl;
}

Response::Response(Response const &rhs) : responses(rhs.responses)
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
		responses = rhs.responses;
	return *this;
}