/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:15:50 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 13:52:41 by llethuil         ###   ########lyon.fr   */
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

void	Server::partCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string>					channelsToLeave;
	std::string									reason = "";
	std::map<std::string, Channel>::iterator	it;

	tokenizer(cmdTokens[1], ",", channelsToLeave);
	if(cmdTokens.size() == 3)
		reason = cmdTokens[2];

	for (size_t i = 0; i < channelsToLeave.size(); i++)
	{
		// IF CHANNEL TO LEAVE DOES NOT EXIST
		if (_channels.find(channelsToLeave[i]) == _channels.end())
		{
			numericReply(user, num.ERR_NOSUCHCHANNEL, channelsToLeave[i], num.MSG_ERR_NOSUCHCHANNEL);
			continue;
		}

		for(it = _channels.begin(); it != _channels.end(); ++it)
		{
			// WHEN CHANNEL IS FIND IN SERVER CHANNELS
			if (it->first == channelsToLeave[i])
			{
				// IF USER IS NOT IN CHANNEL TO LEAVE
				if (user._locations.find(channelsToLeave[i]) == user._locations.end())
				{
					std::string	notInChannelMsg = " " + user._nickname + " " + channelsToLeave[i];
					numericReply(user, num.ERR_NOTONCHANNEL, notInChannelMsg, num.MSG_ERR_NOTONCHANNEL);
					continue ;
				}

				// REMOVE USER FROM CHANNEL MEMBERS
				it->second._members.erase(user._socket);

				// REMOVE CHANNEL FROM USER LOCATIONS
				user._locations.erase(channelsToLeave[i]);

				// REPLY
				cmdReply(user, "PART", channelsToLeave[i] + " " + reason);

				// IF THE USER WAS AN OPERATOR, REMOVE IT FROM OPERATOR SET
				if (it->second._operators.find(user._socket) != it->second._operators.end())
					it->second.removeOperator(user._socket);

				// IF THERE IS NO MEMBERS OR OPERATOR IN CHANNEL ANYMORE, DELETE THE CHANNEL
				if (it->second._members.size() == 0 || it->second._operators.size() == 0)
					deleteChannel(it->first);
			}
		}
	}
}