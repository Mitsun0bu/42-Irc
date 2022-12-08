/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printRecvError.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:36:34 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/08 13:32:21 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/Server.hpp"
# include "../../incs/main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

void	Server::printRecvError(int currentFd)
{
	std::cerr
	<< BLUE << "[SERVER] : " << END
	<< "Connection stopped by " << getUserNickname(currentFd)
	<< " (socket #" << currentFd << ")"
	<< std::endl
	<< std::endl;
}

void	Server::printRecvError(int currentFd, int byteCount)
{
	if (byteCount == 0)
	{
		std::cerr
		<< BLUE << "[SERVER] : " << END
		<< "Connection stopped by " << getUserNickname(currentFd)
		<< " (socket #" << currentFd << ")"
		<< std::endl
		<< std::endl;
	}
}