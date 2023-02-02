/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:47:27 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/02 01:34:29 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <map>

class Response
{

	public:

	Response();
	Response(std::string const &response);
	Response(Response const &rhs);
	~Response();
	Response	&operator=(Response const &rhs);

	private:

	//поля заполненные из запроса

	std::map<std::string, std::string>	Responses;//либо на каждый header своя переменная

};

#endif