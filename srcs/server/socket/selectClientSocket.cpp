/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selectClientSocket.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:34:12 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/08 14:10:08 by llethuil         ###   ########lyon.fr   */
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

	int	ret = select(this->_clientFdList.max + 1, &this->_clientFdList.read, &this->_clientFdList.write, NULL, &this->_clientFdList.t);
	if (ret < 0)
	{
		perror("select()");
		exit(1);
	}

	return ;
}