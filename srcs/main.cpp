/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/26 16:57:20 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"

int	main(int ac, char** av)
{
	std::pair <int, std::string>	param;
	int								serverSocket;

	param = parseArguments(ac, av);

	serverSocket = setServerSocket(param.first);
	if (serverSocket == FAILED)
		exit(1);

	return (0);
}

