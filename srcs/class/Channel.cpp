/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/11 11:45:45 by llethuil         ###   ########lyon.fr   */
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
	_name(name), _key(), _requiresKey(), _topic(), _topicIsSet(false), _members(), _operators(), _mode()
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

	return ;
}

// void	Channel::addMember(int userSocket)
// {
// 	_members.insert(userSocket);
// }

// void	Channel::deleteMember(User &user)
// {
// 	_members.erase(user._socket);
// }

// void	Channel::deleteMember(int userSocket)
// {
// 	_members.erase(userSocket);
// }

// void	Channel::addOperator(User &user)
// {
// 	_operators.insert(user._socket);
// }

// void	Channel::addOperator(int userSocket)
// {
// 	_operators.insert(userSocket);
// }

// void	Channel::deleteOperator(User &user)
// {
// 	_operators.erase(user._socket);
// }

// void	Channel::deleteOperator(int userSocket)
// {
// 	_operators.erase(userSocket);
// }

// void	Channel::sendMsgToMembers(const std::string &msg, int currentSocket) const
// {
// 	std::set<int>::const_iterator	it	= this->_members.begin();
// 	std::set<int>::const_iterator	end	= this->_members.end();

// 	for (it; it != end; ++it)
// 		if (*it != currentSocket)
// 			sendMsg(*it, msg);
// }

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