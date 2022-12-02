/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printRecvError.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:36:34 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/02 16:45:29 by llethuil         ###   ########lyon.fr   */
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

void	Server::printRecvError(int byteCount, int currentFd)
{
	if (byteCount == 0)
	{
		// std::cerr << "Socket " << currentFd << " hung up !" << std::endl;
		std::cerr	<< BLUE << "[SERVER] : " << END
					<< "Connection stopped by "
					<< getUserNickname(currentFd)
					<< " (socket #" << currentFd << ")"
					<< std::endl;
	}
	else
	{
		std::cerr	<< BLUE << "[SERVER] : " << END
					<< "Connection stopped by "
					<< getUserNickname(currentFd)
					<< " (socket #" << currentFd << ")"
					<< std::endl;
		// perror("recv()");
	}
}