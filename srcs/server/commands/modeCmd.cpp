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
	if (cmdTokens.size() < 2)
		return(this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS));

	std::string	target = cmdTokens[1];

	// IF TARGET IS NOT A VALID/EXISTING CHANNEL
	if (findErrorInModeCmd(user, target) == true)
		return ;

	// IF NO MODESTRING IS GIVEN
	if (cmdTokens.size() == 2)
	{
		std::string	modeReply = _channels[target].getModeKey() + _channels[target].getModeInvite();
		cmdReply(user, _num.RPL_CHANNELMODEIS, user.getNickname() + " " + target + " " + modeReply);
	}

	if (cmdTokens.size() >= 3)
		handleModeString(user, cmdTokens, _channels[target]);
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

int		Server::findErrorInModeCmd(User& user, std::string& channelName)
{
	// IF MODE ARGUMENT IS NOT A CHANNEL
	if (checkChannelName(channelName) == FAILED)
		return (true);

	// IF MODE ARGUMENT IS A CHANNEL THAT DOES NOT EXIST IN THE SERVER
	if (_channels.find(channelName) == _channels.end())
	{
		numericReply(user, _num.ERR_NOSUCHCHANNEL, channelName, _num.MSG_ERR_NOSUCHCHANNEL);
		return (true);
	}

	return (false);
}

int		Server::findErrorInModeString(User& user, Channel& channel, std::string modestring)
{
	// IF THE USER IS NOT AN OPERATOR
	if (user.isOperator(channel.getOperators()) == false)
	{
		numericReply(user, _num.ERR_CHANOPRIVSNEEDED, channel.getName(), _num.MSG_ERR_CHANOPRIVSNEEDED);
		return (true);
	}

	// IF THE MODESTRING IS INVALID
	if (modestring.size() != 2 && (modestring[0] != '-' || modestring[0] != '+'))
		return (true);

	return (false);
}

void	Server::handleModeString(User &user, std::vector<std::string> &cmdTokens, Channel& channel)
{
	std::string					modestring = cmdTokens[2];
	std::vector<std::string>	modearguments;

	if (cmdTokens.size() > 3)
		tokenizer(cmdTokens[3], " ", modearguments);

	if (findErrorInModeString(user, channel, modestring) == true)
		return ;

	// HANDLE KEY MODE OR INVITE MODE
	if (modestring[1] == 'k' || modestring[1] == 'i')
	{
		if (modestring[1] == 'k' && handleKeyMode(channel, modestring, modearguments) == FAILED)
			return ;
		else if (modestring[1] == 'i')
			handleInviteMode(channel, modestring);

		std::string	modeReply = channel.getModeKey() + channel.getModeInvite();
		cmdReply(user, _num.RPL_CHANNELMODEIS, user.getNickname() + " " + channel.getName() + " " + modeReply);
		sendCmdToChannel(user, "MODE", channel.getMembers(), channel.getName(), channel.getName() + " " + modestring);

	}

	// HANDLE OPERATOR MODE
	if (modestring[1] == 'o' && modearguments[0].length() != 0)
	{
		if (handleOperatorMode(user, channel, modestring, modearguments) == FAILED)
			return ;

		cmdReply(user, "MODE", channel.getName() + " " + modestring + " " + modearguments[0]);
		sendCmdToChannel(user, "MODE", channel.getMembers(), channel.getName(), channel.getName() + " " + modestring + " " + modearguments[0]);
	}


	return ;
}

int		Server::handleKeyMode(Channel& channel, std::string modestring, std::vector<std::string> modearguments)
{
	if ((modestring[0] == '-' && modearguments.size() != 0) || (modestring[0] == '+' && modearguments.size() == 0))
		return (FAILED);

	if (modestring[0] == '-' && modearguments.size() == 0)
		channel.unsetKey();
	else if (modestring[0] == '+' && modearguments.size() != 0)
		channel.setKey(modearguments[0]);

	return (SUCCESS);
}

int		Server::handleOperatorMode(User& user, Channel& channel, std::string modestring, std::vector<std::string> modearguments)
{
	if (channel.isMember(getUserSocket(modearguments[0])) == false)
		return (FAILED);
	if (modestring[0] == '-')
	{
		channel.removeOperator(getUserSocket(modearguments[0]));
		if (channel.getOperators().size() == 0)
		{
			cmdReply(user, "PART", channel.getName());
			sendCmdToChannel(user, "PART", channel.getMembers(), channel.getName(), "");
			channel.getMembers().erase(getUserSocket(modearguments[0]));
			for (std::set<int>::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); it ++)
			{
				cmdReply(user, "KICK", channel.getName() + " " + getUserNickname(*it));
				sendCmdToChannel(user, "KICK", channel.getMembers(),  channel.getName(), getUserNickname(*it));
				removeUserFromChannel(_users[*it], channel);
				if (channel.getMembers().size() == 0)
					break;
			}
			deleteChannel(channel.getName());
		}
	}
	else if (modestring[0] == '+')
		channel.addOperator(getUserSocket(modearguments[0]));

	return (SUCCESS);
}

void	Server::handleInviteMode(Channel& channel, std::string modestring)
{
	if (modestring[0] == '-')
		channel.unsetModeInvite();
	else if (modestring[0] == '+')
		channel.setModeInvite();
}