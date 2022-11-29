/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:17:53 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/29 10:07:20 by llethuil         ###   ########lyon.fr   */
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

void	Server::kickCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::string msg = cmdTokens[2];

	if (cmdTokens.size() > 3)
		for (size_t i = 3; i < cmdTokens.size(); i++)
			msg.append(" " + cmdTokens[i]);

	if (_channels.find(cmdTokens[1]) == _channels.end())
		numericReply(user, num.ERR_NOSUCHCHANNEL, cmdTokens[1], num.MSG_ERR_NOSUCHCHANNEL);
	else if (_channels[cmdTokens[1]]._operators.find(user._socket) == _channels[cmdTokens[1]]._operators.end())
		numericReply(user, num.ERR_CHANOPRIVSNEEDED, cmdTokens[1], num.MSG_ERR_CHANOPRIVSNEEDED);
	else if (isNickAvailable(cmdTokens[2]))
		numericReply(user, num.ERR_NOSUCHNICK, cmdTokens[2], num.MSG_ERR_NOSUCHNICK);
	else if (_channels[cmdTokens[1]]._members.find(getUserSocket(cmdTokens[2])) == _channels[cmdTokens[1]]._members.end())
		numericReply(user, num.ERR_USERNOTINCHANNEL, cmdTokens[2], num.MSG_ERR_USERNOTINCHANNEL);
	else
	{
		cmdReply(user, "KICK", cmdTokens[1] + " " + msg);
		sendCmdToChannel(user, "KICK", _channels[cmdTokens[1]]._members, cmdTokens[1], msg);
		removeUserFromChannel(_users[getUserSocket(cmdTokens[2])], _channels[cmdTokens[1]]);
	}
}