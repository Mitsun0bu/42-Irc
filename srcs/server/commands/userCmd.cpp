/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:31:50 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 18:22:13 by llethuil         ###   ########lyon.fr   */
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

void	Server::userCmd(User &user, std::vector<std::string> &cmdTokens)
{
	if (!user._validPasswd)
		return ;
	else if (cmdTokens.size() < 4)
		this->numericReply(user, num.ERR_NEEDMOREPARAMS, cmdTokens[0], num.MSG_ERR_NEEDMOREPARAMS);
	else if (user._isAuthenticated)
		this->numericReply(user, num.ERR_ALREADYREGISTERED, num.MSG_ERR_ALREADYREGISTERED);
	else
	{
		user._username = cmdTokens[1];
		if (!user._nickname.empty())
			registerUser(user);
	}
}