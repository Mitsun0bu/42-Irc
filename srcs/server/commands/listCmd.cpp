/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:55:41 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 17:57:57 by llethuil         ###   ########lyon.fr   */
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

void	Server::listCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string>	channelNames;

	if (cmdTokens.size() == 1)
	{
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			std::string	memberCount	= intToStr(it->second._members.size());
			std::string	listMsg		= " " + user._nickname + " " + it->second._name + " " + memberCount + " :" + it->second._topic;
			numericReply(user, num.RPL_LIST, listMsg);
		}
	}
	else
	{
		tokenizer(cmdTokens[1], ",", channelNames);
		for(size_t i = 0; i < channelNames.size(); i++)
		{
			std::string	memberCount	= intToStr(_channels[channelNames[i]]._members.size());
			std::string	topic		= _channels[channelNames[i]]._topic;
			std::string	listMsg		= " " + user._nickname + " " + channelNames[i] + " " + memberCount + " :" + topic;
			numericReply(user, num.RPL_LIST, listMsg);
		}
	}
	numericReply(user, num.RPL_LISTEND, "", num.MSG_RPL_LISTEND);
}