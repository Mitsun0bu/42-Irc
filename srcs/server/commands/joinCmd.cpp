/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:10:37 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/05 15:04:17 by llethuil         ###   ########lyon.fr   */
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

void	Server::joinCmd(User &user, std::vector<std::string> &cmdTokens)
{
	size_t						i		= 0;
	std::vector<std::string>	channelNames;
	std::vector<std::string>	channelKeys;
	std::string					topicMsg;

	if (cmdTokens.size() < 2)
		return(this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS));

	tokenizer(cmdTokens[1], ",", channelNames);
	if(cmdTokens.size() > 2)
		tokenizer(cmdTokens[2], ",", channelKeys);

	for (i = 0; i < channelNames.size(); i++)
	{
		// IF CHANNEL NAME IS INVALID
		if (checkChannelName(channelNames[i]) == FAILED)
		{
			numericReply(user, _num.ERR_NOSUCHCHANNEL, channelNames[i], _num.MSG_ERR_NOSUCHCHANNEL);
			continue ;
		}

		// IF CHANNEL ALREADY EXISTS
		if (_channels.find(channelNames[i]) != _channels.end())
		{
			if (joinExistingChannel(user, i, channelNames, channelKeys) == FAILED)
				continue ;
		}
		// IF CHANNEL DOES NOT EXIST
		else
			joinNewChannel(user, i, channelNames, channelKeys);

		doJoin(user, cmdTokens, i, channelNames);
	}

	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

int		Server::joinExistingChannel(User& user, int i, std::vector<std::string> channelNames, std::vector<std::string> channelKeys)
{
	// IF USER IS ALREADY IN CHANNEL
	if (_channels[channelNames[i]].getMembers().find(user.getSocket()) != _channels[channelNames[i]].getMembers().end())
		return (FAILED);

	// IF THE CHANNEL REQUIRES A KEY AND KEY IS WRONG
	if(_channels[channelNames[i]].getRequiresKey() == true
	&& (channelKeys.size() == 0 || _channels[channelNames[i]].getKey() != channelKeys[i]))
	{
		numericReply(user, _num.ERR_BADCHANNELKEY, channelNames[i], _num.MSG_ERR_BADCHANNELKEY);
		return (FAILED);
	}

	// IF THE CHANNEL IS IN INVITE ONLY MODE AND USER IS NOT ALLOWED
	if (_channels[channelNames[i]].getModeInvite() == "+i"
	&& _channels[channelNames[i]].getAllowedMembers().find(user.getSocket()) == _channels[channelNames[i]].getAllowedMembers().end())
	{
		numericReply(user, _num.ERR_INVITEONLYCHAN, channelNames[i], _num.MSG_ERR_INVITEONLYCHAN);
		return (FAILED);
	}

	_channels[channelNames[i]].getMembers().insert(user.getSocket());
	return (SUCCESS);
}

void	Server::joinNewChannel(User& user, int i, std::vector<std::string> channelNames, std::vector<std::string> channelKeys)
{
	Channel	newChannel(channelNames[i]);

	if (channelKeys.empty() == false && channelKeys[i].length())
		newChannel.setKey(channelKeys[i]);
	newChannel.addMember(user);
	newChannel.addOperator(user);

	addChannel(newChannel, channelNames[i]);
}

void	Server::doJoin(User& user,  std::vector<std::string> &cmdTokens, int i, std::vector<std::string> channelNames)
{
	user.addLocation(channelNames[i]);
	cmdReply(user, "JOIN", channelNames[i]);
	sendCmdToChannel(user, "JOIN", _channels[channelNames[i]].getMembers(), channelNames[i], "");
	replyTopic(user, channelNames[i]);
	namesCmd(user, cmdTokens);
}