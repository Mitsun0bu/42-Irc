/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 11:33:30 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/07 17:43:43 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

int sendMsg(User &user, std::string msg)
{
	msg += "\r\n";
	std::cout << "msg send  : " << msg;
	return (send(user._socket, msg.data(), msg.size(), 0));
}