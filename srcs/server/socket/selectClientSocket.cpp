/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selectClientSocket.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:34:12 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 14:59:37 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/Server.hpp"
# include "../../../incs/main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

void	Server::selectClientSocket(void)
{
	this->_clientFdList.read	= this->_clientFdList.master;
	this->_clientFdList.write	= this->_clientFdList.master;
	if (select(this->_clientFdList.max + 1, &this->_clientFdList.read, &this->_clientFdList.write, NULL, &this->_clientFdList.t) == FAILED)
	{
		perror("select()");
		exit(1);
	}
}