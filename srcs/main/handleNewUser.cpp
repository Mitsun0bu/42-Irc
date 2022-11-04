/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleNewUser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 16:00:49 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/04 11:43:54 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/main.hpp"

User	handleNewUser(Server &s, t_clientFdList *clientFd)
{
	User		newUser;

	newUser._socket = accept(
								s._socket,
								(struct sockaddr *)&newUser._socketAddr,
								&newUser._socketAddrSize
							);

	if (newUser._socket == FAILED)
		perror("accept()");
	else
	{
		FD_SET(newUser._socket, &clientFd->master);

		if (newUser._socket > clientFd->max)
			clientFd->max = newUser._socket;

		// DEBUG
		std::cout	<< "~~~ New connection from "
					<< newUser.getIp()
					<< " on socket "
					<< newUser._socket
					<< " ~~~"
					<< std::endl;
	}

	return (newUser);
}