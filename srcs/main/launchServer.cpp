/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:57:04 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/04 12:05:33 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

void	launchServer(Server &s, t_clientFdList *clientFd)
{
	// CREATE A USER LIST
	std::vector<User>	users;

	while(1)
	{
		clientFd->read = clientFd->master;
		if (select(clientFd->max + 1, &clientFd->read, NULL, NULL, &clientFd->t) == FAILED)
		{
			perror("select()");
			exit(1);
		}

		// DEBUG
		if (users.size() > 1)
		{
			std::cout << "CONNECTED USER :" << users[0]._socket << std::endl;
			std::cout << "CONNECTED USER :" << users[1]._socket << std::endl;
		}

		// RUN THROUGH THE EXISTING CONNECTION LOOKING FOR DATA

		searchConnection(); // Create this funciton in Server class and put User list in server too

		for (int fd = 0; fd <= clientFd->max; fd++)
		{
			// ONE FD IS SENDING DATA TO THE SERVER
			if (FD_ISSET(fd, &clientFd->read))
			{
				if (fd == s._socket)
				{
					User	newUser;
					newUser = handleNewUser(s, clientFd);
					users.push_back(newUser);
				}
				else
					handleClientData(s, clientFd, &fd);
			}
		}
	}
}