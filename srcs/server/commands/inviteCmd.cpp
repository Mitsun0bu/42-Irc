/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:08:07 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/08 17:40:36 by llethuil         ###   ########lyon.fr   */
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
	if (cmdTokens.size() < 3)
		return(this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS));

	if (cmdTokens.size() != 3)
		return ;

	std::string	userInviting	= user.getNickname();
	std::string	userToInvite	= cmdTokens[1];
	std::string	channelName		= cmdTokens[2];

	if (handleInviteError(user, channelName, userToInvite) == FAILED)
		return ;

	// IF USER TO INVITE IS ALREADY A CHANNEL MEMBER
	if (_channels[channelName].isMember(getUserSocket(userToInvite)) == true)
	{
		std::string	inChannelMsg = " " + userToInvite + " " + channelName;
		numericReply(user, _num.ERR_USERONCHANNEL, inChannelMsg, _num.MSG_ERR_USERONCHANNEL);
		return ;
	}

	numericReply(user, _num.RPL_INVITING, " " + user.getNickname() + " " + userToInvite + " " + channelName);

	sendInvitation(userInviting, userToInvite, channelName);

	if (_channels[channelName].getModeInvite() == "+i")
		_channels[channelName].getAllowedMembers().insert(getUserSocket(userToInvite));

	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

int		Server::handleInviteError(User &user, std::string channelName, std::string userToInvite)
{
	// IF CHANNEL DOES NOT EXIST
	if (channelExists(channelName) == false)
	{
		numericReply(user, _num.ERR_NOSUCHCHANNEL, channelName, _num.MSG_ERR_NOSUCHCHANNEL);
		return (FAILED);
	}

	// IF USER TO INVITE DOES NOT EXIST
	if (getUserSocket(userToInvite) == FAILED)
	{
		numericReply(user, _num.ERR_NOSUCHNICK, userToInvite, _num.MSG_ERR_NOSUCHNICK);
		return (FAILED);
	}

	// IF INVITING USER IS NOT A CHANNEL MEMBER
	if (_channels[channelName].isMember(user.getSocket()) == false)
	{
		std::string	notInChannelMsg = " " + user.getNickname() + " " + channelName;
		numericReply(user, _num.ERR_NOTONCHANNEL, notInChannelMsg, _num.MSG_ERR_NOTONCHANNEL);
		return (FAILED);
	}

	// IF CHANNEL IS IN INVITE-ONLY MODE AND INVITING USER IS NOT A CHANNEL OPERATOR
	if (_channels[channelName].getModeInvite() == "+i"
	&& user.isOperator(_channels[channelName].getOperators()) == false)
	{
		numericReply(user, _num.ERR_CHANOPRIVSNEEDED, channelName, _num.MSG_ERR_CHANOPRIVSNEEDED);
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