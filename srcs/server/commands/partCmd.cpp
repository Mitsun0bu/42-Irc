/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:15:50 by llethuil          #+#    #+#             */
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

	if (cmdTokens.size() < 2)
		return(this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS));

	for (size_t i = 0; i < channelsToLeave.size(); i++)
	{
		// IF CHANNEL TO LEAVE DOES NOT EXIST
		if (_channels.find(channelsToLeave[i]) == _channels.end())
		{
			numericReply(user, _num.ERR_NOSUCHCHANNEL, channelsToLeave[i], _num.MSG_ERR_NOSUCHCHANNEL);
			continue;
		}

		for(it = _channels.begin(); it != _channels.end(); ++it)
		{
			// WHEN CHANNEL IS FIND IN SERVER CHANNELS
			if (it->first == channelsToLeave[i])
			{
				// IF USER IS NOT IN CHANNEL TO LEAVE
				if (user.getLocations().find(channelsToLeave[i]) == user.getLocations().end())
				{
					std::string	notInChannelMsg = " " + user.getNickname() + " " + channelsToLeave[i];
					numericReply(user, _num.ERR_NOTONCHANNEL, notInChannelMsg, _num.MSG_ERR_NOTONCHANNEL);
					continue ;
				}
				// REMOVE USER FROM CHANNEL MEMBERS
				it->second.getMembers().erase(user.getSocket());

				// REMOVE CHANNEL FROM USER LOCATIONS
				user.getLocations().erase(channelsToLeave[i]);
				// REPLY
				cmdReply(user, "PART", channelsToLeave[i] + " " + reason);

				// IF THE USER WAS AN OPERATOR, REMOVE IT FROM OPERATOR SET
				if (it->second.getOperators().find(user.getSocket()) != it->second.getOperators().end())
					it->second.removeOperator(user.getSocket());
				// IF THERE IS NO MEMBERS OR OPERATOR IN CHANNEL ANYMORE, DELETE THE CHANNEL
				if (it->second.getMembers().size() == 0 || it->second.getOperators().size() == 0)
					deleteChannel(it->first);
			}
		}
	}
}