/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:09:50 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 19:15:05 by agirardi         ###   ########lyon.fr   */
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

void	Server::privmsgCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string> targets;
	std::string msg;

	if (cmdTokens.size() < 3)
		return(numericReply(user, _num.ERR_NOTEXTTOSEND, _num.MSG_ERR_NOTEXTTOSEND));

	for (size_t i = 2; i < cmdTokens.size(); i++)
		msg.append(cmdTokens[i]);

	tokenizer(cmdTokens[1], ",", targets);
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].find("#") != std::string::npos)
			sendMsgToChannel(user, targets[i], msg);
		else
			sendMsgToUser(user, targets[i], msg);
	}
}