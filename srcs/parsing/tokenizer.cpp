/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:06:16 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/10 10:18:59 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/main.hpp"

void	tokenizer(std::string str, const char* del, std::vector<std::string> &tokens)
{
	// DEBUG
	// std::cout << "TO TOKENIZE : " << str << std::endl;

	char*	token = strtok(const_cast<char*>(str.c_str()), del);
	while (token != nullptr)
	{
		// DEBUG
		// std::cout << "TOKEN : " << token << std::endl;
		tokens.push_back(std::string(token));
		token = strtok(nullptr, del);
	}
	// DEBUG
	// std::cout << "--------------------" << std::endl;


}