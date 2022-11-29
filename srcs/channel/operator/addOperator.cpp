/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addOperator.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:52:17 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/29 10:12:57 by llethuil         ###   ########lyon.fr   */
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

void	Channel::addOperator(int userSocket)
{
	_operators.insert(userSocket);
}

void	Channel::addOperator(User &user)
{
	_operators.insert(user._socket);
}