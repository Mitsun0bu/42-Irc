/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsUsers.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:58:24 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 14:53:56 by llethuil         ###   ########lyon.fr   */
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
/*                               ~~~ FUNCTIONS ~~~                            */
/*                                                                            */
/* ************************************************************************** */

void  Server::acceptNewUser(void)
{
	int						socket;
	struct sockaddr_storage	socketAddr;
	socklen_t				socketAddrSize = 0;

	socket = accept(this->_socket, (struct sockaddr*)&socketAddr, &socketAddrSize);
	User &newUser = this->_users[socket];

	newUser._socketAddr = socketAddr;
	newUser._socketAddrSize = socketAddrSize;
	newUser.setSocket(socket);

	if (newUser.getSocket() == FAILED)
	{
		perror("accept()");
		this->_users.erase(socket);
	}
	else
	{
		FD_SET(newUser.getSocket(), &this->_clientFdList.master);
		if (newUser.getSocket() > this->_clientFdList.max)
			this->_clientFdList.max = newUser.getSocket();
	}
}

int		Server::getUserSocket(std::string &nickname)
{
	std::map<int, User>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second._nickname == nickname)
			return (it->second._socket);
	}
	return (FAILED);
}

bool	Server::isNickAvailable(std::string &nickname)
{
	std::map<int, User>::iterator	it;

	for (it = this->_users.begin(); it != this->_users.end(); it++)
			if (nickname == it->second._nickname)
				return (false);
	return (true);
}

void	Server::registerUser(User &user)
{
	user._isAuthenticated = true;
	this->numericReply(user, num.RPL_WELCOME, user._nickname, num.MSG_RPL_WELCOME, user._nickname);
	this->numericReply(user, num.RPL_YOURHOST, user._nickname, num.MSG_RPL_YOURHOST);
	this->numericReply(user, num.RPL_CREATED, user._nickname, num.MSG_RPL_CREATED, _date);
	this->numericReply(user, num.RPL_MYINFO, user._nickname, num.MSG_RPL_MYINFO);
}

void	Server::logoutUser(User &user)
{
	std::map<std::string, Channel>::iterator it = _channels.begin();

	while (it != _channels.end())
	{
		if (it->second._operators.find(user._socket) != it->second._operators.end())
			it->second._operators.erase(user._socket);
		if (it->second._members.find(user._socket) != it->second._members.end())
		{
			it->second._members.erase(user._socket);
			if (it->second._members.size() == 0)
			{
				std::map<std::string, Channel>::iterator toErase = it;
				++it;
				_channels.erase(toErase);
			}
			else
				++it;
		}
	}
	this->_users.erase(user._socket);
}