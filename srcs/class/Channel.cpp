/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/22 10:44:22 by llethuil         ###   ########lyon.fr   */
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
	_name(name), _key(), _requiresKey(), _topic(), _topicIsSet(false), _members(), _operators(), _mode("-k")
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Channel has been created !"
				<< std::endl;

	return ;
}

Channel::Channel(Channel const & src)
{
	std::cout	<< ORANGE
				<< "[COPY CONSTRUCTOR] : "
				<< END
				<< "A Channel has been duplicated !"
				<< std::endl;

	*this = src;

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
	this->_mode			= src._mode;

	return (*this);
}

/* ************************************************************************** */
/*                                                                            */
/*                         ~~~ MEMBER FUNCTIONS ~~~                           */
/*                                                                            */
/* ************************************************************************** */

void	Channel::addMember(User &user)
{
	this->_members.insert(user._socket);
}

void	Channel::setKey(std::string key)
{
	this->_key			= key;
	this->_requiresKey	= true;
	this->_mode			= "+k";

	return ;
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