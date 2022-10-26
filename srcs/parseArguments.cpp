/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseEntries.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:52:54 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/26 14:08:30 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

std::pair<int, std::string> parseArguments(int ac, char** av)
{
	std::pair<int, std::string> entries;

	if (ac != 3)
	{
		std::cerr << "Error : Invalid arguments !" << std::endl
				  << "Usage : ./ircserv [port] [password]";
		exit(1);
	}
	else
	{
		entries = std::make_pair(atoi(av[1]), av[2]);
		return (entries);
	}
}