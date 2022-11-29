/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   removeOperator.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:52:19 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/29 10:13:01 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/Channel.hpp"
# include "../../../incs/main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

void	Channel::removeOperator(int userSocket)
{
	std::set<int>::iterator operatorIterator;

	for(operatorIterator = _operators.begin(); operatorIterator != _operators.end(); ++operatorIterator)
	{
		if(*operatorIterator == userSocket)
		{
			_operators.erase(userSocket);
			return ;
		}
	}
	return ;
}
