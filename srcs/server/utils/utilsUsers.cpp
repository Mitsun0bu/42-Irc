/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsUsers.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:58:24 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 01:19:30 by agirardi         ###   ########lyon.fr   */
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

	newUser.setSocketAddr(socketAddr);
	newUser.setSocketAddrSize(socketAddrSize);
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
		if (it->second.getNickname() == nickname)
			return (it->second.getSocket());
	}
	return (FAILED);
}

bool	Server::isNickAvailable(std::string &nickname)
{
	std::map<int, User>::iterator	it;

	for (it = this->_users.begin(); it != this->_users.end(); it++)
			if (it->second.getNickname() == nickname)
				return (false);
	return (true);
}

void	Server::registerUser(User &user)
{
	user.setIsAuthenticated(true);
	this->numericReply(user, _num.RPL_WELCOME, user.getNickname(), _num.MSG_RPL_WELCOME, user.getNickname());
	this->numericReply(user, _num.RPL_YOURHOST, user.getNickname(), _num.MSG_RPL_YOURHOST);
	this->numericReply(user, _num.RPL_CREATED, user.getNickname(), _num.MSG_RPL_CREATED, _date);
	this->numericReply(user, _num.RPL_MYINFO, user.getNickname(), _num.MSG_RPL_MYINFO);
}

void	Server::logoutUser(User &user)
{
	std::map<std::string, Channel>::iterator it = _channels.begin();

	while (it != _channels.end())
	{
		if (it->second.getOperators().find(user.getSocket()) != it->second.getOperators().end())
			it->second.getOperators().erase(user.getSocket());
		if (it->second.getMembers().find(user.getSocket()) != it->second.getMembers().end())
		{
			it->second.getMembers().erase(user.getSocket());
			if (it->second.getMembers().size() == 0)
			{
				std::map<std::string, Channel>::iterator toErase = it;
				++it;
				_channels.erase(toErase);
			}
			else
				++it;
		}
	}
	this->_users.erase(user.getSocket());
}