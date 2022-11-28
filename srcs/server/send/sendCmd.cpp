/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:42:54 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 18:43:15 by llethuil         ###   ########lyon.fr   */
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
/*                               ~~~ MAIN FUNCTION ~~~                        */
/*                                                                            */
/* ************************************************************************** */

void	Server::sendCmdToUser(User &from, std::string cmd, User &target, std::string msg)
{
	std::string finalMsg = ":" + from._nickname + " " + cmd + " " + target._nickname + " " + msg + "\r\n";

	if (FD_ISSET(target._socket, &this->clientFdList.write))
		if (send(target._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::sendCmdToChannel(User &from, std::string cmd, std::set<int> &targets, std::string channel, std::string msg)
{
	std::string finalMsg = ":" + from._nickname + " " + cmd + " " + channel + " " + msg + "\r\n";
	std::set<int>::iterator it;

	for (it = targets.begin(); it != targets.end(); ++it)
	{
		if (_users[*it]._socket != from._socket && FD_ISSET(_users[*it]._socket, &this->clientFdList.write))
			if (send(_users[*it]._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
				perror("send()");
	}
}