/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:14:38 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 18:30:39 by llethuil         ###   ########lyon.fr   */
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

void	Server::sendMsgToUser(User &sender, std::string &target, std::string &msg)
{
	int targetSocket = getUserSocket(target);

	if (targetSocket == FAILED && _cmdMap[sender._cmdToExecute] == "PRIVMSG")
		numericReply(sender, num.ERR_NOSUCHNICK, target, num.MSG_ERR_NOSUCHNICK);
	else
		sendCmdToUser(sender, _cmdMap[sender._cmdToExecute], _users[targetSocket], msg);
}

void	Server::sendMsgToChannel(User &sender, std::string &target, std::string &msg)
{
	std::string channelName = target.substr(target.find("#"), std::string::npos);
	std::string	&cmd = _cmdMap[sender._cmdToExecute];

	if (_channels.find(channelName) == _channels.end() && cmd == "PRIVMSG")
		return(numericReply(sender, num.ERR_NOSUCHNICK, target, num.MSG_ERR_NOSUCHNICK));

	Channel &channel = _channels[channelName];
	if	((!parseTargetPrefix(target) || !checkUserPermissions(sender, channel)) && cmd == "PRIVMSG")
		return(numericReply(sender, num.ERR_CANNOTSENDTOCHAN, target, num.MSG_ERR_CANNOTSENDTOCHAN));

	if (target[0] == '@')
		sendCmdToChannel(sender, cmd, channel._operators, channelName, msg);
	else
		sendCmdToChannel(sender, cmd, channel._members, channelName, msg);
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

bool	Server::parseTargetPrefix(const std::string &target)
{
	size_t pos = target.find("#");

	if (pos > 1)
		return (false);
	if (pos == 1 && target[0] != '@')
		return (false);
	return (true);
}

bool	Server::checkUserPermissions(User &user, Channel &channel)
{
	if (channel._bannedMembers.find(user._socket) != channel._bannedMembers.end() ||
			channel._members.find(user._socket) == channel._members.end())
		return (false);
	return (true);
}