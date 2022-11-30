/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:08:07 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 14:59:37 by llethuil         ###   ########lyon.fr   */
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

void	Server::inviteCmd(User &user, std::vector<std::string> &cmdTokens)
{
	if (cmdTokens.size() != 3)
		return ;

	std::string	userInviting	= user._nickname;
	std::string	userToInvite	= cmdTokens[1];
	std::string	channelName		= cmdTokens[2];

	if (handleInviteError(user, channelName) == FAILED)
		return ;

	// IF USER TO INVITE IS ALREADY A CHANNEL MEMBER
	if (_channels[channelName].isMember(getUserSocket(userToInvite)) == true)
	{
		std::string	inChannelMsg = " " + userToInvite + " " + channelName;
		numericReply(user, num.ERR_USERONCHANNEL, inChannelMsg, num.MSG_ERR_USERONCHANNEL);
		return ;
	}

	numericReply(user, num.RPL_INVITING, " " + user._nickname + " " + userToInvite + " " + channelName);
	sendInvitation(userInviting, userToInvite, channelName);
	if (_channels[channelName]._modeInvite == "+i")
		_channels[channelName]._allowedMembers.insert(getUserSocket(userToInvite));

	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

int		Server::handleInviteError(User &user, std::string channelName)
{
	// IF CHANNEL DOES NOT EXIST
	if (channelExists(channelName) == false)
	{
		numericReply(user, num.ERR_NOSUCHCHANNEL, channelName, num.MSG_ERR_NOSUCHCHANNEL);
		return (FAILED);
	}

	// IF INVITING USER IS NOT A CHANNEL MEMBER
	if (_channels[channelName].isMember(user._socket) == false)
	{
		std::string	notInChannelMsg = " " + user._nickname + " " + channelName;
		numericReply(user, num.ERR_NOTONCHANNEL, notInChannelMsg, num.MSG_ERR_NOTONCHANNEL);
		return (FAILED);
	}

	// IF CHANNEL IS IN INVITE-ONLY MODE AND INVITING USER IS NOT A CHANNEL OPERATOR
	if (_channels[channelName]._modeInvite == "+i"
	&& user.isOperator(_channels[channelName]._operators) == false)
	{
		numericReply(user, num.ERR_CHANOPRIVSNEEDED, channelName, num.MSG_ERR_CHANOPRIVSNEEDED);
		return (FAILED);
	}

	return (SUCCESS);
}

void	Server::sendInvitation(std::string userInviting, std::string userToInvite, std::string channelName)
{
	std::string	invitationMsg	= ":" + userInviting + " INVITE " + userToInvite + " " + channelName + "\r\n";
	int			socket			= getUserSocket(userToInvite);

	if (FD_ISSET(socket, &this->_clientFdList.write))
		if (send(socket, invitationMsg.c_str(), invitationMsg.size(), 0) == FAILED)
			perror("send()");
}