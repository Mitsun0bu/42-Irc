/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:14:38 by llethuil          #+#    #+#             */
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

void	Server::sendMsgToUser(User &sender, std::string &target, std::string &msg)
{
	int targetSocket = getUserSocket(target);

	if (targetSocket == FAILED && _cmdMap[sender.getCmdToExecute()] == "PRIVMSG")
		numericReply(sender, _num.ERR_NOSUCHNICK, target, _num.MSG_ERR_NOSUCHNICK);
	else
		sendCmdToUser(sender, _cmdMap[sender.getCmdToExecute()], _users[targetSocket], msg);
}

void	Server::sendMsgToChannel(User &sender, std::string &target, std::string &msg)
{
	std::string channelName = target.substr(target.find("#"), std::string::npos);
	std::string	&cmd = _cmdMap[sender.getCmdToExecute()];

	if (_channels.find(channelName) == _channels.end() && cmd == "PRIVMSG")
		return(numericReply(sender, _num.ERR_NOSUCHNICK, target, _num.MSG_ERR_NOSUCHNICK));

	Channel &channel = _channels[channelName];
	if	((!parseTargetPrefix(target) || !checkUserPermissions(sender, channel)) && cmd == "PRIVMSG")
		return(numericReply(sender, _num.ERR_CANNOTSENDTOCHAN, target, _num.MSG_ERR_CANNOTSENDTOCHAN));

	if (target[0] == '@')
		sendCmdToChannel(sender, cmd, channel.getOperators(), channelName, msg);
	else
		sendCmdToChannel(sender, cmd, channel.getMembers(), channelName, msg);
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
	if (channel.getBannedMembers().find(user.getSocket()) != channel.getBannedMembers().end() ||
			channel.getMembers().find(user.getSocket()) == channel.getMembers().end())
		return (false);
	return (true);
}