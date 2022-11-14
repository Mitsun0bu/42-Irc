/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:06:16 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/14 03:14:14 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/main.hpp"

void	tokenizer(std::string str, const char *del, std::vector<std::string> &tokens)
{
	char*	token = strtok(const_cast<char*>(str.c_str()), del);
	while (token != nullptr)
	{
		tokens.push_back(std::string(token));
		token = strtok(nullptr, del);
	}
}