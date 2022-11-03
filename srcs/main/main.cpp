/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/03 15:22:41 by llethuil         ###   ########lyon.fr   */
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

	// CLIENT-RELATED VARIABLES
	int							fdmax				= 0;
	fd_set						fdMaster;
	fd_set						fdBuff;
	FD_ZERO(&fdMaster);
	FD_ZERO(&fdBuff);
	struct timeval				tv;
	tv.tv_sec										= 2;
	tv.tv_usec										= 500000;

	// CREATE SERVER, SET SERVER AND ADD SERVER TO THE MASTER FDs SET
	Server	s(port, addressFamily, socketType, socketFlag, socketBlockingMode, protocol, internetHostAddr);

	if (s.setSocket() == FAILED)
		exit(1);

	FD_SET(s._socket, &fdMaster);
	fdmax											= s._socket;

	// CREATE USER BUFFER AND USER LIST
	User				userBuff;
	std::vector<User>	users;

	std::cout << std::endl << "~~~ Waiting for new connection ~~~" << std::endl;

	// HANDLE CLIENT CONNECTION
	while(1)
	{
		fdBuff = fdMaster;
		if (select(fdmax + 1, &fdBuff, NULL, NULL, &tv) == FAILED)
		{
			perror("select()");
			exit(1);
		}

		// RUN THROUGH THE EXISTING CONNECTION LOOKING FOR DATA
		for (int i = 0; i <= fdmax; i++)
		{
			// ONE FD IS SENDING DATA TO THE SERVER
			if (FD_ISSET(i, &fdBuff))
			{
				if (i == s._socket)
				{
					// HANDLE NEW CONNECTIONS
					userBuff._socket = accept(s._socket, (struct sockaddr *)&userBuff._socketAddr, &userBuff._socketAddrSize);
					if (userBuff._socket != FAILED)
						perror("accept()");
					else
					{
						FD_SET(userBuff._socket, &fdMaster);

						if (userBuff._socket > fdmax)
							fdmax = userBuff._socket;

						char		remoteIP[INET6_ADDRSTRLEN];
						const void*	inAddr	= getInAddr((struct sockaddr*)&userBuff._socketAddr);
						const char*	inet	= inet_ntop(userBuff._socketAddr.ss_family, inAddr, remoteIP, INET6_ADDRSTRLEN);

						std::cout	<< "~~~ New connection from "
									<< inet
									<< " on socket "
									<< userBuff._socket
									<< " ~~~";

						users.push_back(User(userBuff));
					}
				}
				else
				{
					//HANDLE DATA FROM A CLIENT

					char	clientDataBuffer[256];
					int		byteCount = 0;

					if ((byteCount = recv(i, clientDataBuffer, sizeof clientDataBuffer, 0)) <= 0)
					{
						// ERROR OR CONNECTION CLOSED BY THE CLIENT
						if (byteCount == 0)
						{
							std::cout	<< "Socket "
										<< i
										<< "hung up !"
										<< std::endl;
						}
						else
							perror("recv()");
						close(i);
						FD_CLR(i, &fdMaster);
					}
					else
					{
						// HANDLE DATA FROM THE CLIENT
						for(int j = 0; j <= fdmax; j++)
						{
							// SEND IT TO EVERYONE
							if (FD_ISSET(j, &fdMaster))
							{
								// EXCEPT TO THE SERVER AND CURRENT CLIENT
								if (j != s._socket && j != i)
								{
									if (send(j, clientDataBuffer, byteCount, 0) == -1)
										perror("send");
								}
							}
						}
					}

					// bzero(buffer, 512);

					// byteCount = recv(userBuff._socket, buffer, 512, 0);
					// std::cout << "Here is the message: " << buffer << std::endl;
					// byteCount += recv(userBuff._socket, buffer, 512, 0);
					// std::cout << "Here is the message: " << buffer << std::endl;
					// byteCount += recv(userBuff._socket, buffer, 512, 0);
					// std::cout << "Here is the message: " << buffer << std::endl;
					// std::cout	<< "recv()'d "
					// 			<< byteCount
					// 			<< " bytes of data."
					// 			<< std::endl;
				}
			}
		}
	}

	return (0);
}