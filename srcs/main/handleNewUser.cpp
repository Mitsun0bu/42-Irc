/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleNewUser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 16:00:49 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/03 17:52:06 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/main.hpp"

User	handleNewUser(Server &s, fd_set* fdMaster, int* fdmax)
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
		FD_SET(newUser._socket, fdMaster);

		if (newUser._socket > *fdmax)
			*fdmax = newUser._socket;

		std::cout	<< "~~~ New connection from "
					<< newUser.getIp()
					<< " on socket "
					<< newUser._socket
					<< " ~~~"
					<< std::endl;
	}

	return (newUser);
}