/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainAlex.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/03 09:47:22 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"
# include <vector>
# include "../../incs/Server.hpp"
# include "../../incs/User.hpp"

// regarder select()

int	main(int ac, char** av)
{
	std::pair<int, std::string>	param				= parseArguments(ac, av);

	int							port				= param.first;
	std::string					passwd				= param.second;

	int							addressFamily		= AF_INET;
	int							socketType			= SOCK_STREAM;
	int							socketFlag			= F_SETFL;
	int							socketBlockingMode	= O_NONBLOCK;
	int							protocol			= 0;
	const char*					internetHostAddr	= "127.0.0.1";

	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 500000;

	fd_set	readfds;
	int			fdmax = 0;
	Server	s(port, addressFamily, socketType, socketFlag, socketBlockingMode, protocol, internetHostAddr);

	if (s.setSocket() == FAILED)
		exit(1);

	FD_ZERO(&readfds);
	FD_SET(s._socket, &readfds);

	fdmax = s._socket;

	std::vector<User>	users;
	User	b;


	std::cout << std::endl << "~~~ Waiting for new connection ~~~" << std::endl;


	while(1)
	{

		if (select(fdmax + 1, &readfds, NULL, NULL, &tv) == FAILED) {
			perror("select");
				exit(1);
		}

			std::cout << "fdmax: " << fdmax << std::endl;


		for (int i = 0; i <= fdmax; i++)
		{
			std::cout << "i: " << i << std::endl;
			if (FD_ISSET(i, &readfds))
			{
				std::cout << "ICI" << std::endl;
				if (i == s._socket)
				{
					if ((b._socket = accept(s._socket, (struct sockaddr *)&b._socketAddr, &b._socketAddrSize)) != FAILED)
					{
						std::cout << std::endl << "~~~ New connection established~~~" << std::endl;

						FD_SET(b._socket, &readfds);
						fdmax += 1;

						users.push_back(User(b));
					}
				}
			}
		}

	}

	// CLOSE THE serverSocket
	std::cout << "Server closed" << std::endl;
	close(s._socket);

	return (0);
}

