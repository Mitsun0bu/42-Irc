/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserGetters.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:24:17 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 15:04:01 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/User.hpp"

struct sockaddr_storage	User::getSocketAddr(void) const
{
		return (_socketAddr); 
}

socklen_t								User::getSocketAddrSize(void) const
{
	return (_socketAddrSize); 
}

int											User::getSocket(void) const
{
	return (_socket);
}

bool										User::getValidPasswd(void) const
{
	return (_validPasswd);
}

bool										User::getIsAuthenticated(void) const
{
	return (_isAuthenticated);
}

std::set<std::string>		User::getLocations(void) const
{
	return (_locations);
}

std::string							User::getIp(void) const
{
	return (_ip);
}

std::string 						User::getNickname(void) const
{
	return (_nickname);
}

std::string 						User::getUsername(void) const
{
	return (_username);
}

std::string							User::getCmdReceived(void) const
{
	return (_cmdReceived);
}

int											User::getCmdToExecute(void) const
{
	return (_cmdToExecute);
}
