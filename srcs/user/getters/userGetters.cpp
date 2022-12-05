/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userGetters.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:24:17 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/05 15:02:44 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/User.hpp"

struct sockaddr_storage	User::getSocketAddr(void) const
{
		return (_socketAddr);
}

socklen_t	User::getSocketAddrSize(void) const
{
	return (_socketAddrSize);
}

int	User::getSocket(void) const
{
	return (_socket);
}

bool	User::getValidPasswd(void) const
{
	return (_validPasswd);
}

bool	User::getIsAuthenticated(void) const
{
	return (_isAuthenticated);
}

std::set<std::string>&	User::getLocations(void)
{
	return (_locations);
}

std::string&	User::getNickname(void)
{
	return (_nickname);
}

std::string&	User::getUsername(void)
{
	return (_username);
}

std::string&	User::getCmdReceived(void)
{
	return (_cmdReceived);
}

int	User::getCmdToExecute(void) const
{
	return (_cmdToExecute);
}
