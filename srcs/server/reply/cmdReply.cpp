/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdReply.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:32:44 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 18:33:11 by llethuil         ###   ########lyon.fr   */
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
/*                               ~~~ MAIN FUNCTION ~~~                        */
/*                                                                            */
/* ************************************************************************** */

void	Server::cmdReply(User &user, std::string cmd, std::string param)
{
	std::string finalMsg = ":" + user._nickname + " " + cmd + " " + param + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}