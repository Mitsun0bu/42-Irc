/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleClientData.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 09:50:52 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/04 11:51:02 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

static void	printRecvError(int byteCount, int currentFd);
static void	sendClientData(Server &s, t_clientFdList *clientFd, int* currentFd, char* buffer, int byteCount);

// HANDLE DATA FROM THE CLIENT
void	handleClientData(Server &s, t_clientFdList *clientFd, int* currentFd)
{
	char	buffer[256]	= {0};
	int		byteCount	= 0;

	byteCount = recv(*currentFd, buffer, sizeof buffer, 0);
	if (byteCount <= 0)
	{
		printRecvError(byteCount, *currentFd);
		close(*currentFd);
		FD_CLR(*currentFd, &clientFd->master);
	}
	else
		sendClientData(s, clientFd, currentFd, buffer, byteCount);
}

// PRINT AN ERROR MESSAGE IF :
//		- THERE IS A RECEIV() ERROR
//		- THE CONNECTION IS CLOSED BY THE CLIENT
static void	printRecvError(int byteCount, int currentFd)
{
	if (byteCount == 0)
			std::cerr << "Socket " << currentFd << " hung up !" << std::endl;
	else
		perror("recv()");
}

// SEND DATA FROM THE CLIENT TO EVERYONE EXCEPT SERVER AND CURRENT CLIENT
static void	sendClientData(Server &s, t_clientFdList *clientFd, int* currentFd, char* buffer, int byteCount)
{
	for(int fd = 0; fd <= clientFd->max; fd++)
		if (FD_ISSET(fd, &clientFd->master))
			if (fd != s._socket && fd != *currentFd)
				if (send(fd, buffer, byteCount, 0) == -1)
					perror("send()");
}