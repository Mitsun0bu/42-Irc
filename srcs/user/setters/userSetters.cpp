/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserSetters.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:24:17 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 15:31:20 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/User.hpp"

void	User::setSocketAddr(struct sockaddr_storage socketAddr)
{
	_socketAddr = socketAddr;
}

void	User::setSocketAddrSize(socklen_t socketAddrSize)
{
	_socketAddrSize = socketAddrSize;

}

void	User::setSocket(int socket)
{
	_socket = socket;
}

void	User::setValidPasswd(bool isValid)
{
	_validPasswd = isValid;
}

void	User::setIsAuthenticated(bool isAuthenticated)
{
	_isAuthenticated = isAuthenticated;
}

void	User::setLocations(std::set<std::string> locations)
{
	_locations = locations;
}

void	User::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void	User::setUsername(std::string username)
{
	_username = username;
}

void	User::setCmdReceived(std::string cmd)
{
	_cmdReceived = cmd;
}

void	User::setCmdToExecute(int cmd)
{
	_cmdToExecute = cmd;
}
