/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 15:12:01 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/Channel.hpp"
# include "../../incs/main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                      ~~~ CONSTRUCTORS ~~~                                  */
/*                                                                            */
/* ************************************************************************** */

Channel::Channel(void)
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Channel has been created !"
				<< std::endl;

	return ;
}

Channel::Channel(std::string name) :
	_name(name), _key(), _requiresKey(), _topic(), _topicIsSet(false), _members(), _operators(), _modeKey("-k"), _modeInvite("-i")
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Channel has been created !"
				<< std::endl;

	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                         ~~~ OPERATOR OVERLOAD ~~~                          */
/*                                                                            */
/* ************************************************************************** */

Channel&	Channel::operator=(Channel const & src)
{
	this->_name			= src._name;
	this->_key			= src._key;
	this->_requiresKey	= src._requiresKey;
	this->_topic		= src._topic;
	this->_topicIsSet	= src._topicIsSet;
	this->_members		= src._members;
	this->_operators	= src._operators;
	this->_modeKey		= src._modeKey;
	this->_modeInvite	= src._modeInvite;

	return (*this);
}

/* ************************************************************************** */
/*                                                                            */
/*                           ~~~ DESTRUCTOR ~~~                               */
/*                                                                            */
/* ************************************************************************** */

Channel::~Channel(void)
{
	std::cout	<< PURPLE
				<< "[DESTRUCTOR] : "
				<< END
				<< "A Channel has been closed."
				<< std::endl;

	return ;
}