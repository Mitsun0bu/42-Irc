/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 11:33:30 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 00:30:11 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

int sendMsg(User &user, std::string msg)
{
	msg += "\r\n";
	std::cout << "msg send  : " << msg;
	return (send(user.getSocket(), msg.data(), msg.size(), 0));
}