/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:26:46 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 17:27:30 by llethuil         ###   ########lyon.fr   */
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

void	Server::passCmd(User &user, std::vector<std::string> &cmdTokens)
{
	if (user._isAuthenticated)
		this->numericReply(user, _num.ERR_ALREADYREGISTERED, _num.MSG_ERR_ALREADYREGISTERED);
	else if (cmdTokens.size() < 2)
		this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS);
	else if (cmdTokens[1] != this->_passwd)
	{
		this->numericReply(user, _num.ERR_PASSWDMISMATCH, cmdTokens[1], _num.MSG_ERR_PASSWDMISMATCH);
		this->errorReply(user, "Authentication failed");
	}
	else
		user._validPasswd = true;
}