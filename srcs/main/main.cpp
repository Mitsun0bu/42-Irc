/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/02 16:48:23 by llethuil         ###   ########lyon.fr   */
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
	const char* 				internetHostAddr	= "127.0.0.1";

	Server	s(port, addressFamily, socketType, socketFlag, socketBlockingMode, protocol, internetHostAddr);

	if (s.setSocket() == FAILED)
		exit(1);

	User	b;

	std::cout << std::endl << "~~~ Waiting for new connection ~~~" << std::endl;

	while(1)
	{
		if ((b._socket = accept(s._socket, (struct sockaddr *)&b._socketAddr, &b._socketAddrSize)) != FAILED)
		{
			std::cout << std::endl << "~~~ New connection established~~~" << std::endl;

			std::vector<User>	users;
			users.push_back(b);

			char	buffer[512];
			int		byteCount = 0;

			bzero(buffer, 512);

			byteCount = recv(b._socket, buffer, 512, 0);
			std::cout << "Here is the message: " << buffer << std::endl;
			byteCount += recv(b._socket, buffer, 512, 0);
			std::cout << "Here is the message: " << buffer << std::endl;
			byteCount += recv(b._socket, buffer, 512, 0);
			std::cout << "Here is the message: " << buffer << std::endl;
			std::cout	<< "recv()'d "
						<< byteCount
						<< " bytes of data."
						<< std::endl;
		}

	}

	// CLOSE THE serverSocket
	close(s._socket);

	return (0);
}

