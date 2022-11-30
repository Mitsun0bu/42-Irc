/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:07:17 by llethuil          #+#    #+#             */
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

void	Server::handleCmd(User &user, std::vector<std::string> &cmdTokens)
{
	user._cmdToExecute = this->findCmdToExecute(cmdTokens[0]);

	if (!user._isAuthenticated && user._cmdToExecute > 4)
		return(this->numericReply(user, _num.ERR_NOTREGISTERED, cmdTokens[0], _num.MSG_ERR_NOTREGISTERED));

	switch(user._cmdToExecute)
	{
		case PASS:
			this->passCmd(user, cmdTokens);
			break;
		case CAP:
			break;
		case NICK:
			this->nickCmd(user, cmdTokens);
			break;
		case USER:
			this->userCmd(user, cmdTokens);
			break;
		case PING:
			this->handlePing(user, cmdTokens);
			break;
		case QUIT:
			this->quitCmd(user, cmdTokens);
			break;
		case JOIN:
			this->joinCmd(user, cmdTokens);
			break;
		case PART:
			this->partCmd(user, cmdTokens);
			break;
		case TOPIC:
			this->topicCmd(user, cmdTokens);
			break;
		case NAMES:
			this->namesCmd(user, cmdTokens);
			break;
		case LIST:
			this->listCmd(user, cmdTokens);
			break;
		case INVITE:
			this->inviteCmd(user, cmdTokens);
			break;
		case KICK:
			this->kickCmd(user, cmdTokens);
			break;
		case MODE:
			this->modeCmd(user, cmdTokens);
			break;
		case PRIVMSG:
		case NOTICE:
			this->privmsgCmd(user, cmdTokens);
			break;
		default:
			this->numericReply(user, _num.ERR_UNKNOWNCOMMAND, cmdTokens[0], _num.MSG_ERR_UNKNOWNCOMMAND);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

int		Server::findCmdToExecute(std::string &cmd)
{
	std::map<int, std::string>::iterator it;

	for (it = _cmdMap.begin(); it != _cmdMap.end(); it++)
	{
		if (cmd == it->second)
			return it->first;
	}
	return (FAILED);

}