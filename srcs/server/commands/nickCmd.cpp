/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:25:40 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 17:32:55 by llethuil         ###   ########lyon.fr   */
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

void	Server::nickCmd(User &user, std::vector<std::string> &cmdTokens)
{

	if (!user.getValidPasswd())
		return ;
	else if (cmdTokens.size() < 2)
		this->numericReply(user, _num.ERR_NONICKNAMEGIVEN, _num.MSG_ERR_NONICKNAMEGIVEN);
	else if (!isNickAvailable(cmdTokens[1]))
		this->numericReply(user, _num.ERR_NICKNAMEINUSE, cmdTokens[1], _num.MSG_ERR_NICKNAMEINUSE);
	else if (!parseNick(cmdTokens[1]))
		this->numericReply(user, _num.ERR_ERRONEUSNICKNAME, cmdTokens[1], _num.MSG_ERR_ERRONEUSNICKNAME);
	else
	{
		if (user.getIsAuthenticated())
		{
			this->cmdReply(user, "NICK", cmdTokens[1]);
			notifyAllChannels(user, cmdTokens[1]);
		}
		user.setNickname(cmdTokens[1]);
		if (!user.getUsername().empty() && !user.getIsAuthenticated())
			registerUser(user);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

void	Server::notifyAllChannels(User &user, std::string &nickname)
{
	std::set<std::string>::iterator it;

	for(it = user.getLocations().begin(); it != user.getLocations().end(); ++it)
		sendCmdToChannel(user, "NICK", _channels[*it].getMembers(), "", nickname);
}    

bool	Server::parseNick(std::string &nickname)
{
	std::string validSpecialCharset = "-_[]{}\\`|";

	if (nickname.size() > 9)
		return (false);
	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (!isalnum(nickname[i]) && validSpecialCharset.find(nickname[i]) == std::string::npos)
			return (false);
	}
	return (true);
}