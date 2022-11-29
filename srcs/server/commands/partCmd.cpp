/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:15:50 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/29 10:07:44 by llethuil         ###   ########lyon.fr   */
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
	std::vector<std::string>	channelNames;
	std::string					reason = "";
	std::map<std::string, Channel>::iterator it;

	tokenizer(cmdTokens[1], ",", channelNames);
	if(cmdTokens.size() == 3)
		reason = cmdTokens[2];

	for (size_t i = 0; i < channelNames.size(); i++)
	{
		if (_channels.find(channelNames[i]) == _channels.end())
		{
			numericReply(user, num.ERR_NOSUCHCHANNEL, channelNames[i], num.MSG_ERR_NOSUCHCHANNEL);
			continue;
		}

		for(it = _channels.begin(); it != _channels.end(); ++it)
		{
			// HANDLE OPERATORS
			// if (it->second._operators.find(user._socket) != it->second._operators.end())
			// 	it->second._operators.erase(user._socket);

			if (it->first == channelNames[i])
			{
				if (user._locations.find(channelNames[i]) == user._locations.end())
				{
					std::string	notInChannelMsg = " " + user._nickname + " " + channelNames[i];
					numericReply(user, num.ERR_NOTONCHANNEL, notInChannelMsg, num.MSG_ERR_NOTONCHANNEL);
					continue ;
				}

				it->second._members.erase(user._socket);
				user._locations.erase(channelNames[i]);
				cmdReply(user, "PART", channelNames[i] + " " + reason);

				if (it->second._members.size() == 0)
				{
					std::map<std::string, Channel>::iterator toErase;
					toErase = it;
					_channels.erase(toErase);
					if (_channels.size() == 0)
					{
						_channels.clear();
						return ;
					}
				}
			}
		}
	}
}