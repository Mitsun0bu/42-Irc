/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:24:17 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/29 10:03:21 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/User.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                      ~~~ CONSTRUCTORS ~~~                                  */
/*                                                                            */
/* ************************************************************************** */

User::User(void) : _isAuthenticated(false), _cmdToExecute(-1)
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A new user has been created !"
				<< std::endl;

	this->_socketAddrSize	= sizeof(this->_socketAddr);

	return ;
}

User::User(User const & src)
{
	std::cout	<< ORANGE
				<< "[COPY CONSTRUCTOR] : "
				<< END
				<< "A user has been duplicated !"
				<< std::endl;

	*this = src;

	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                         ~~~ OPERATOR OVERLOAD ~~~                          */
/*                                                                            */
/* ************************************************************************** */

User&	User::operator=(User const & src)
{
	this->_socket			= src._socket;
	this->_socketAddr		= src._socketAddr;
	this->_socketAddrSize	= src._socketAddrSize;
	strcpy(this->_remoteIP, src._remoteIP);
	this->_inAddr = src._inAddr;
	this->_ip = src._ip;



	return (*this);
}

/* ************************************************************************** */
/*                                                                            */
/*                           ~~~ DESTRUCTOR ~~~                               */
/*                                                                            */
/* ************************************************************************** */

User::~User(void)
{
	std::cout	<< PURPLE
				<< "[DESTRUCTOR] : "
				<< END
				<< "User "
				<< this->_socket
				<< " has been destroyed."
				<< std::endl;
	return ;
}