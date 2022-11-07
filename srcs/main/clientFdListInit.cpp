/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientFdListInit.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:28:18 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/07 17:34:49 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

void	clientFdListInit(t_fdList *clientFdList, int serverSocket)
{
	clientFdList->max 		= serverSocket;
	clientFdList->t.tv_sec	= 0;
	clientFdList->t.tv_usec	= 10;
	FD_ZERO(&clientFdList->master);
	FD_ZERO(&clientFdList->read);
	FD_ZERO(&clientFdList->write);
	FD_SET(serverSocket, &clientFdList->master);
}