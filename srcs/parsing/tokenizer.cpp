/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:06:16 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/10 16:07:47 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/main.hpp"

void	tokenizer(std::string str, std::string del, std::vector<std::string> &tokens)
{
	std::string delimiter = del;
	std::cout << "str :|" << str << "|" << std::endl; // debug

	char*	token = strtok(const_cast<char*>(str.c_str()), const_cast<char*>(delimiter.c_str()));
	while (token != nullptr)
	{
		// std::cout << "token : |" << token << "|" << std::endl;
		tokens.push_back(std::string(token));
		token = strtok(nullptr, const_cast<char*>(delimiter.c_str()));
	}
}