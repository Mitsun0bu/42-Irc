/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bindServerSocket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:52:47 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/31 14:57:22 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/main.hpp"

int	bindServerSocket(int serverSocket, struct sockaddr_in& serverSocketAddr)
{
	int				res = 0;
	unsigned int	sizeAddr = sizeof(serverSocketAddr);

	res = bind(serverSocket, (const struct sockaddr *)&serverSocketAddr, sizeAddr);
	if (res == FAILED)
	{
		std::cerr << "Error : Server socket cannot bind to address !" << std::endl;
		close(serverSocket);
		return (FAILED);
	}
	// else
	// 	res = getsockname(serverSocket, (struct sockaddr *) &serverSocketAddr, &sizeAddr);

	// CHECK IF getsockname() & setsockopt() are useful //

	return (res);
}