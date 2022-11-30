/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:10:37 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 15:17:32 by llethuil         ###   ########lyon.fr   */
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

	tokenizer(cmdTokens[1], ",", channelNames);
	if(cmdTokens.size() > 2)
		tokenizer(cmdTokens[2], ",", channelKeys);

	for (i = 0; i < channelNames.size(); i++)
	{
		if (checkChannelName(channelNames[i]) == FAILED)
		{
			numericReply(user, _num.ERR_NOSUCHCHANNEL, channelNames[i], _num.MSG_ERR_NOSUCHCHANNEL);
			continue ;
		}
		// IF CHANNEL ALREADY EXISTS
		else if (_channels.find(channelNames[i]) != _channels.end())
		{
			// IF USER IS ALREADY IN CHANNEL
			if (_channels[channelNames[i]]._members.find(user._socket) != _channels[channelNames[i]]._members.end())
				return ;
			// IF THE CHANNEL REQUIRES A KEY AND KEY IS WRONG
			if(_channels[channelNames[i]]._requiresKey == true
			&& (channelKeys.size() == 0 || _channels[channelNames[i]]._key != channelKeys[i]))
			{
				numericReply(user, _num.ERR_BADCHANNELKEY, channelNames[i], _num.MSG_ERR_BADCHANNELKEY);
				return ;
			}
			// IF THE CHANNEL IS IN INVITE ONLY MODE AND USER IS NOT ALLOWED
			if (_channels[channelNames[i]]._modeInvite == "+i"
			&& _channels[channelNames[i]]._allowedMembers.find(user._socket) == _channels[channelNames[i]]._allowedMembers.end())
			{
				numericReply(user, _num.ERR_INVITEONLYCHAN, channelNames[i], _num.MSG_ERR_INVITEONLYCHAN);
				return ;
			}
			_channels[channelNames[i]]._members.insert(user._socket);
		}
		// IF CHANNEL DOES NOT EXIST
		else
		{
			Channel	newChannel(channelNames[i]);

			if (channelKeys.empty() == false && channelKeys[i].length())
				newChannel.setKey(channelKeys[i]);
			newChannel.addMember(user);
			newChannel.addOperator(user);

			addChannel(newChannel, channelNames[i]);
		}

		user.addLocation(channelNames[i]);
		cmdReply(user, "JOIN", channelNames[i]);
		sendCmdToChannel(user, "JOIN", _channels[channelNames[i]]._members, channelNames[i], "");
		replyTopic(user, channelNames[i]);
		namesCmd(user, cmdTokens);
	}
}