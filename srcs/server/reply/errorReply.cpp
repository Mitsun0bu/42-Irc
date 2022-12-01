/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorReply.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:38:24 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 00:30:11 by agirardi         ###   ########lyon.fr   */
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

void	Server::errorReply(User &user, std::string reason)
{
	std::string cmd = "Error :" + reason + "\r\n";
	if (FD_ISSET(user.getSocket(), &this->_clientFdList.write))
		if (send(user.getSocket(), cmd.c_str(), cmd.size(), 0) == FAILED)
			perror("send");
}