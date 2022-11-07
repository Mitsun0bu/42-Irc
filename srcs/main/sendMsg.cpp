/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 11:33:30 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/07 11:35:12 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

int sendMsg(const int socket, std::string msg)
{
	msg += "\r\n";
	std::cout << "msg send  : " << msg;
	return (send(socket, msg.data(), msg.size(), 0));
}