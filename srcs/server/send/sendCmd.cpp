/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:42:54 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 00:39:50 by agirardi         ###   ########lyon.fr   */
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

void	Server::sendCmdToUser(User &from, std::string cmd, User &target, std::string msg)
{
	std::string finalMsg = ":" + from.getNickname() + " " + cmd + " " + target.getNickname() + " " + msg + "\r\n";

	if (FD_ISSET(target.getSocket(), &this->_clientFdList.write))
		if (send(target.getSocket(), finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::sendCmdToChannel(User &from, std::string cmd, std::set<int> &targets, std::string channel, std::string msg)
{
	std::string finalMsg = ":" + from.getNickname() + " " + cmd + " " + channel + " " + msg + "\r\n";
	std::set<int>::iterator it;

	for (it = targets.begin(); it != targets.end(); ++it)
	{
		if (_users[*it].getSocket() != from.getSocket() && FD_ISSET(_users[*it].getSocket(), &this->_clientFdList.write))
			if (send(_users[*it].getSocket(), finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
				perror("send()");
	}
}