/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnathali <mnathali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 00:34:33 by mnathali          #+#    #+#             */
/*   Updated: 2023/02/02 12:18:33 by mnathali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{

	public:

	Request();
	Request(std::string const &request);
	Request(Request const &rhs);
	~Request();
	Request	&operator=(Request const &rhs);

	private:

	//поля заполненные из запроса

	std::map<std::string, std::string>	requests;//либо на каждый header своя переменная
	std::string							message;

};

#endif