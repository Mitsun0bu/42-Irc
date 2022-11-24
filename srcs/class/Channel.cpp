/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/24 16:57:13 by llethuil         ###   ########lyon.fr   */
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
	_members.insert(user._socket);
}

bool	Channel::isMember(int userSocket)
{
	if (_members.find(userSocket) == _members.end())
		return (false);
	else
		return (true);
}

void	Channel::addOperator(int userSocket)
{
	_operators.insert(userSocket);
}

void	Channel::addOperator(User &user)
{
	_operators.insert(user._socket);
}

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

// void	Channel::removeOperator(User &user)
// {
// 	std::set<int>::iterator operatorIterator;

// 	for(operatorIterator = _operators.begin(); operatorIterator != _operators.end(); ++operatorIterator)
// 	{
// 		if(*operatorIterator == user._socket)
// 		{
// 			_operators.erase(user._socket);
// 			return ;
// 		}
// 	}
// 	return ;
// }

void	Channel::setKey(std::string key)
{
	_key			= key;
	_requiresKey	= true;
	_mode			= "+k";

	return ;
}

void	Channel::unsetKey()
{
	_key.clear();
	_requiresKey	= false;
	_mode			= "-k";

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