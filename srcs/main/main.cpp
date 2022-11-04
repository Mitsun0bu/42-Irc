/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/04 12:02:40 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

int	main(int ac, char** av)
{
	// HANDLE PROGRAM ARGUMENTS
	std::pair<int, std::string>	param				= parseArguments(ac, av);
	int							port				= param.first;
	std::string					passwd				= param.second;

	// SERVER-RELATED VARIABLES
	int							addressFamily		= AF_INET;
	int							socketType			= SOCK_STREAM;
	int							socketFlag			= F_SETFL;
	int							socketBlockingMode	= O_NONBLOCK;
	int							protocol			= 0;
	const char*					internetHostAddr	= "127.0.0.1";

	// CREATE SERVER
	Server	server(
					port,
					addressFamily,
					socketType,
					socketFlag,
					socketBlockingMode,
					protocol,
					internetHostAddr
			 	  );

	// SET SERVER
	if (server.setSocket() == FAILED)
		exit(1);

	// CLIENT-RELATED FILE DESCRIPTORS
	t_clientFdList	clientFd;
	clientFdListInit(&clientFd, server._socket);

	// DEBUG
	std::cout << std::endl << "~~~ Waiting for new connection ~~~" << std::endl;

	// HANDLE CLIENT CONNECTION
	launchServer(server, &clientFd);

	return (0);
}