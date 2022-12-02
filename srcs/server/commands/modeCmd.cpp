/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:03:22 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 18:05:48 by llethuil         ###   ########lyon.fr   */
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

void	Server::modeCmd(User& user, std::vector<std::string> &cmdTokens)
{
	// DEBUG
	for(size_t i = 0; i < cmdTokens.size(); i++)
		std::cout << cmdTokens[i] << std::endl;

	if (cmdTokens.size() < 2)
		return(this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS));

	std::string	target = cmdTokens[1];

	// IF TARGET IS NOT A VALID/EXISTING CHANNEL
	if (handleChannelModeError(user, target) == FAILED)
		return ;

	// IF NO MODESTRING IS GIVEN
	if (cmdTokens.size() == 2)
	{
		std::string	modeString = _channels[target].getModeKey() + _channels[target].getModeInvite();
		cmdReply(user, _num.RPL_CHANNELMODEIS, user.getNickname() + " " + target + " " + modeString);
	}

	if (cmdTokens.size() >= 3)
		handleModeString(user, cmdTokens, _channels[target]);
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

int		Server::handleChannelModeError(User& user, std::string& channelName)
{
	// CHECK IF MODE ARGUMENT IS A CHANNEL
	if (checkChannelName(channelName) == FAILED)
		return (FAILED);

	// CHECK IF MODE ARGUMENT IS A CHANNEL THAT EXISTS IN THE SERVER
	if (_channels.find(channelName) == _channels.end())
	{
		numericReply(user, _num.ERR_NOSUCHCHANNEL, channelName, _num.MSG_ERR_NOSUCHCHANNEL);
		return (FAILED);
	}

	return (SUCCESS);
}

void	Server::handleModeString(User &user, std::vector<std::string> &cmdTokens, Channel& channel)
{
	std::string					modestring = cmdTokens[2];
	std::vector<std::string>	modearguments;

	if (cmdTokens.size() > 3)
		tokenizer(cmdTokens[3], " ", modearguments);

	// IF THE USER IS AN OPERATOR
	if (user.isOperator(channel.getOperators()) == false)
	{
		numericReply(user, _num.ERR_CHANOPRIVSNEEDED, channel.getName(), _num.MSG_ERR_CHANOPRIVSNEEDED);
		return ;
	}

	// IF THE MODESTRING IS VALID
	if (modestring.size() != 2 && (modestring[0] != '-' || modestring[0] != '+'))
		return ;

	// HANDLE CHANNEL KEY MODE
	if (modestring[1] == 'k')
	{
		if ((modestring[0] == '-' && modearguments.size() != 0)
		||	(modestring[0] == '+' && modearguments.size() == 0))
			return ;

		if (modestring[0] == '-' && modearguments.size() == 0)
			channel.unsetKey();
		else if (modestring[0] == '+' && modearguments.size() != 0)
			channel.setKey(modearguments[0]);

		std::string	modeString = channel.getModeKey() + channel.getModeInvite();
		cmdReply(user, _num.RPL_CHANNELMODEIS, user.getNickname() + " " + channel.getName() + " " + modeString);
	}

	// HANDLE CHANNEL OPERATOR MODE
	if (modestring[1] == 'o' && modearguments[0].length() != 0)
	{
		if (channel.isMember(getUserSocket(modearguments[0])) == false)
			return ;
		if (modestring[0] == '-')
		{
			channel.removeOperator(getUserSocket(modearguments[0]));
			if (channel.getOperators().size() == 0)
				deleteChannel(channel.getName());
		}
		else if (modestring[0] == '+')
			channel.addOperator(getUserSocket(modearguments[0]));

		cmdReply(user, "MODE", channel.getName() + " " + modestring + " " + modearguments[0]);
	}

	// HANDLE INVITE ONLY MODE
	if (modestring[1] == 'i')
	{
		if (modestring[0] == '-')
			channel.unsetModeInvite();
		else if (modestring[0] == '+')
			channel.setModeInvite();

		std::string	modeString = channel.getModeKey() + channel.getModeInvite();
		cmdReply(user, _num.RPL_CHANNELMODEIS, user.getNickname() + " " + channel.getName() + " " + modeString);
	}

	return ;
}