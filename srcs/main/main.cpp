/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/31 18:13:15 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

int	main(int ac, char** av)
{
	std::pair <int, std::string> param = parseArguments(ac, av);

	int serverSocket = setServerSocket(param.first);
	if (serverSocket == FAILED)
		exit(1);

	std::string passwd = param.second;

	int new_socket;
	struct sockaddr_storage their_address;
	socklen_t addr_size = sizeof(their_address);

	while(1)
	{
		std::cout << std::endl << "~~~ Waiting for new connection ~~~" << std::endl;

		if ((new_socket = accept(serverSocket, (struct sockaddr *)&their_address, &addr_size)) == FAILED)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cout << std::endl << "~~~ New connection established~~~" << std::endl;

			char buffer[1024];
			int n = 0;
			bzero(buffer,1024);
			n = read(new_socket, buffer, 1024);
			if (n < 0)
				std::cout << "ERROR reading from socket";
			std::cout << "Here is the message: " << buffer ;
		}

	}

	// CLOSE THE serverSocket
	close(serverSocket);

	return (0);
}

