/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:32:28 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/31 14:56:32 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/main.hpp"

int	main(int ac, char** av)
{
	std::pair <int, std::string> param = parseArguments(ac, av);

	int serverSocket = setServerSocket(param.first);
	if (serverSocket == FAILED)
		exit(1);

	std::string passwd = param.second;

	return (0);
}

