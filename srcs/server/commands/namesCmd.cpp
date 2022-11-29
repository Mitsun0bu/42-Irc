/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   namesCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:16:35 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/29 10:07:34 by llethuil         ###   ########lyon.fr   */
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

void	Server::namesCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::string					nick = user._nickname;
	std::vector<std::string>	channelNames;
	tokenizer(cmdTokens[1], ",", channelNames);


	for(size_t i = 0; i < channelNames.size(); i++)
	{
		//IF CHANNEL NAME IS INVALID
		if (checkChannelName(channelNames[i]) == FAILED)
		{
			this->numericReply(user, num.RPL_ENDOFNAMES, nick + " " + channelNames[i], num.MSG_RPL_ENDOFNAMES);
			continue ;
		}
		// IF CHANNEL EXISTS
		if(this->_channels.find(channelNames[i]) != this->_channels.end())
		{
			std::string namesMsg = " = " + channelNames[i] + " :";
			// ADD CHANNEL MEMBERS' NICKNAME TO MESSAGE
			std::set<int>	members = this->_channels[channelNames[i]]._members;
			for (std::set<int>::iterator fd = members.begin(); fd != members.end(); fd++)
			{
				if (_channels[channelNames[i]]._operators.find(*fd) != _channels[channelNames[i]]._operators.end())
					namesMsg += "@" + this->_users[*fd]._nickname;
				else
					namesMsg += this->_users[*fd]._nickname;
				namesMsg += " ";
			}
			std::cout << "namesMsg: " << namesMsg << std::endl;
			this->numericReply(user, num.RPL_NAMREPLY, namesMsg);
		}
		this->numericReply(user, num.RPL_ENDOFNAMES, nick + " " + channelNames[i], num.MSG_RPL_ENDOFNAMES);
	}
}