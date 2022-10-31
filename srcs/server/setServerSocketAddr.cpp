/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setServerSocketAddr.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:06:03 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/31 14:57:42 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/main.hpp"

void setServerSocketAddr(struct sockaddr_in& serverSocketAddr, const char* internetHostAddress, int port)
{
	bzero(&serverSocketAddr, sizeof(serverSocketAddr));
	serverSocketAddr.sin_addr.s_addr	= inet_addr(internetHostAddress);
	serverSocketAddr.sin_family			= AF_INET;
	serverSocketAddr.sin_port			= htons(port);
}