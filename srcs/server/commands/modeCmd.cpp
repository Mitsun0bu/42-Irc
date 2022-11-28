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
/*                               ~~~ MAIN FUNCTION ~~~                        */
/*                                                                            */
/* ************************************************************************** */

void	Server::modeCmd(User& user, std::vector<std::string> &cmdTokens)
{
	for(size_t i = 0; i < cmdTokens.size(); i++)
		std::cout << cmdTokens[i] << std::endl;

	std::string	target = cmdTokens[1];

	// IF TARGET IS NOT A VALID/EXISTING CHANNEL
	if (handleChannelModeError(user, target) == FAILED)
		return ;

	// IF NO MODESTRING IS GIVEN
	if (cmdTokens.size() == 2)
		numericReply(user, num.RPL_CHANNELMODEIS, " " + target + " " + _channels[target]._modeKey + _channels[target]._modeInvite);

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
		numericReply(user, num.ERR_NOSUCHCHANNEL, channelName, num.MSG_ERR_NOSUCHCHANNEL);
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

	// CHECK IF THE USER IS AN OPERATOR
	if (user.isOperator(channel._operators) == false)
	{
		numericReply(user, num.ERR_CHANOPRIVSNEEDED, channel._name, num.MSG_ERR_CHANOPRIVSNEEDED);
		return ;
	}

	// CHECK IF THE MODESTRING IS VALID
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

		numericReply(user, num.RPL_CHANNELMODEIS, " " + channel._name + " " + channel._modeKey);
	}

	// HANDLE CHANNEL OPERATOR MODE
	if (modestring[1] == 'o' && modearguments[0].length() != 0)
	{
		if (channel.isMember(getUserSocket(modearguments[0])) == false)
			return ;
		if (modestring[0] == '-')
		{
			channel.removeOperator(getUserSocket(modearguments[0]));
			if (channel._operators.size() == 0)
				deleteChannel(channel._name);
		}
		else if (modestring[0] == '+')
			channel.addOperator(getUserSocket(modearguments[0]));
		cmdReply(user, "MODE", channel._name + " " + modestring + " " + modearguments[0]);
	}

	// HANDLE INVITE ONLY MODE
	if (modestring[1] == 'i')
	{
		if (modestring[0] == '-')
			channel._modeInvite = "-i";
		else if (modestring[0] == '+')
			channel._modeInvite = "+i";
	}

	return ;
}