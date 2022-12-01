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
	if (!user.getValidPasswd())
		return ;
	else if (user.getIsAuthenticated())
		this->numericReply(user, _num.ERR_ALREADYREGISTERED, _num.MSG_ERR_ALREADYREGISTERED);
	else if (cmdTokens.size() < 4)
		this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS);
	else
	{
		user.setUsername(cmdTokens[1]);
		if (!user.getNickname().empty())
			registerUser(user);
	}
}