/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pingCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 19:15:05 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/09 15:01:32 by agirardi         ###   ########lyon.fr   */
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

void	Server::handlePing(User &user, std::vector<std::string> &cmdTokens)
{
	if (cmdTokens.size() < 2)
		return(this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS));

	std::string host = _internetHostAddr;
	cmdReply(user, "PONG", host + " " + cmdTokens[1]);
}
