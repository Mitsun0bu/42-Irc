/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setServerSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:42:45 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/31 14:57:40 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/main.hpp"

int setServerSocket(int port)
{
	//	ALLOCATE A SOCKET FILE DESCRIPTOR (endpoint for communication)	//
	int	serverSocket = socket(AF_INET, SOCK_STREAM, 0);


	//	SET THE ADDRESS OF THE serverSocket								//
	struct sockaddr_in	serverSocketAddr;
	const char*			internetHostAddress = "127.0.0.1";
	setServerSocketAddr(serverSocketAddr, internetHostAddress, port);


	// BIND THE serverSocket TO serverSocketAddr						//
	if (bindServerSocket(serverSocket, serverSocketAddr) == FAILED)
		return (FAILED);

	// LISTEN TO THE port ON THE serverSocket							//
	if (listen(serverSocket, SOMAXCONN) == FAILED)
	{
		std::cerr << "Error : Server socket cannot listen to the targeted port !" << std::endl;
		return (FAILED);
	}

	return (serverSocket);
}