/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsUsers.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:58:24 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/08 14:28:56 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/Server.hpp"
# include "../../incs/main.hpp"

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

	// DEBUG
	std::cout << "HERE" << std::endl;

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

int		Server::getUserSocket(std::string& nickname)
{
	std::map<int, User>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second.getNickname() == nickname)
			return (it->second.getSocket());
	}
	return (FAILED);
}

std::string	Server::getUserNickname(int socket)
{
	std::map<int, User>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second.getSocket() == socket)
			return (it->second.getNickname());
	}
	return ("");
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
	std::set<std::string>	copyLocations(user.getLocations());
	std::set<std::string>::iterator it;

	for(it = copyLocations.begin(); it != copyLocations.end(); ++it)
		leaveChannel(user, _channels[*it], *it, "");
	FD_CLR(user.getSocket(), &_clientFdList.master);
	close(user.getSocket());
	this->_users.erase(user.getSocket());
}