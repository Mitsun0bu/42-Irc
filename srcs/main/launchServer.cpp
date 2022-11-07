/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:57:04 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/07 16:08:36 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

void	launchServer(Server &server, t_fdList *clientFdList)
{
	while(1)
	{
		server.selectClientSocket(clientFdList);
		server.searchForData(clientFdList);
	}
}