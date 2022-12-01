/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientFdListInit.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:41:27 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 01:26:00 by agirardi         ###   ########lyon.fr   */
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

void	Server::clientFdListInit(void)
{
	this->_clientFdList.max			= this->_socket;
	this->_clientFdList.t.tv_sec		= 0;
	this->_clientFdList.t.tv_usec	= 10;
	FD_ZERO(&this->_clientFdList.master);
	FD_ZERO(&this->_clientFdList.read);
	FD_ZERO(&this->_clientFdList.write);
	FD_SET(this->_socket, &this->_clientFdList.master);
}