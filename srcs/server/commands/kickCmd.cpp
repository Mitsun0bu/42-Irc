/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:17:53 by llethuil          #+#    #+#             */
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

void	Server::kickCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::string msg = cmdTokens[2];

	if (cmdTokens.size() > 3)
		for (size_t i = 3; i < cmdTokens.size(); i++)
			msg.append(" " + cmdTokens[i]);

	if (_channels.find(cmdTokens[1]) == _channels.end())
		numericReply(user, _num.ERR_NOSUCHCHANNEL, cmdTokens[1], _num.MSG_ERR_NOSUCHCHANNEL);
	else if (_channels[cmdTokens[1]].getOperators().find(user.getSocket()) == _channels[cmdTokens[1]].getOperators().end())
		numericReply(user, _num.ERR_CHANOPRIVSNEEDED, cmdTokens[1], _num.MSG_ERR_CHANOPRIVSNEEDED);
	else if (isNickAvailable(cmdTokens[2]))
		numericReply(user, _num.ERR_NOSUCHNICK, cmdTokens[2], _num.MSG_ERR_NOSUCHNICK);
	else if (_channels[cmdTokens[1]].getMembers().find(getUserSocket(cmdTokens[2])) == _channels[cmdTokens[1]].getMembers().end())
		numericReply(user, _num.ERR_USERNOTINCHANNEL, cmdTokens[2], _num.MSG_ERR_USERNOTINCHANNEL);
	else
	{
		cmdReply(user, "KICK", cmdTokens[1] + " " + msg);
		sendCmdToChannel(user, "KICK", _channels[cmdTokens[1]].getMembers(), cmdTokens[1], msg);
		removeUserFromChannel(_users[getUserSocket(cmdTokens[2])], _channels[cmdTokens[1]]);
	}
}