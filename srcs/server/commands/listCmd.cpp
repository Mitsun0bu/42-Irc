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
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

void	Server::listCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string>	channelNames;

	if (cmdTokens.size() == 1)
	{
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			std::string	memberCount	= intToStr(it->second.getMembers().size());
			std::string	listMsg		= " " + user.getNickname() + " " + it->second.getName() + " " + memberCount + " :" + it->second.getTopic();
			numericReply(user, _num.RPL_LIST, listMsg);
		}
	}
	else
	{
		tokenizer(cmdTokens[1], ",", channelNames);
		for(size_t i = 0; i < channelNames.size(); i++)
		{
			std::string	memberCount	= intToStr(_channels[channelNames[i]].getMembers().size());
			std::string	topic		= _channels[channelNames[i]].getTopic();
			std::string	listMsg		= " " + user.getNickname() + " " + channelNames[i] + " " + memberCount + " :" + topic;
			numericReply(user, _num.RPL_LIST, listMsg);
		}
	}
	numericReply(user, _num.RPL_LISTEND, "", _num.MSG_RPL_LISTEND);
}