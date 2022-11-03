/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getInAddr.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 10:46:39 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/03 10:55:13 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/main.hpp"

const void*	getInAddr(struct sockaddr *socketAddr)
{
	if (socketAddr->sa_family == AF_INET)
		return &(((struct sockaddr_in*)socketAddr)->sin_addr);

	return &(((struct sockaddr_in6*)socketAddr)->sin6_addr);
}