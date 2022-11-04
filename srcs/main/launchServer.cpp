/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:57:04 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/04 13:59:25 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

void	launchServer(Server &server, t_fdList *clientFdList)
{
	while(1)
	{
		server.selectClientSocket(clientFdList);

		// DEBUG
		if (server._users.size() > 1)
		{
			std::cout << "CONNECTED USER :" << server._users[0]._socket << std::endl;
			std::cout << "CONNECTED USER :" << server._users[1]._socket << std::endl;
		}

		server.searchForData(clientFdList);
	}
}