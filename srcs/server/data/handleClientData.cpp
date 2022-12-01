/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleClientData.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:37:51 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 01:05:26 by agirardi         ###   ########lyon.fr   */
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
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

void	Server::handleClientData(int &currentFd)
{
	char						buffer[4080]	= {0};
	int							byteCount	= 0;
	std::vector<std::string>	cmds;
	std::vector<std::string>	cmdTokens;
	User &currentUser = _users[currentFd];

	byteCount = recv(currentFd, buffer, 4080, 0);
	currentUser.setCmdReceived(currentUser.getCmdReceived() += buffer);
	if (byteCount <= 0)
	{
		this->printRecvError(byteCount, currentFd);
		if (_users.find(currentFd) != _users.end())
			logoutUser(_users[currentFd]);
		close(currentFd);
		FD_CLR(currentFd, &this->_clientFdList.master);
	}
	else
	{
		if (currentUser.getCmdReceived()[currentUser.getCmdReceived().length() - 1] != '\n')
			return ;
		tokenizer(currentUser.getCmdReceived(), "\r\n", cmds);
		currentUser.setCmdReceived("");

		for(size_t i = 0; i < cmds.size(); i ++)
		{
			tokenizer(cmds[i], " ", cmdTokens);
			this->handleCmd(currentUser, cmdTokens);
		}
	}
}