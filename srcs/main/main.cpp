/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/05 15:06:41 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

int	main(int ac, char** av)
{
	// HANDLE PROGRAM ARGUMENTS
	std::pair<int, std::string>	param	= parseArguments(ac, av);
	int							port	= param.first;
	std::string					passwd	= param.second;

	// CREATE SERVER
	Server	server(
					port,
					passwd,
					AF_INET,
					SOCK_STREAM,
					F_SETFL,
					O_NONBLOCK,
					0,
					"127.0.0.1"
			 	  );

	// SET SERVER
	if (server.setSocket() == FAILED)
		exit(1);
	server.clientFdListInit();

	// HANDLE CLIENT CONNECTION
	launchServer(server);

	return (0);
}