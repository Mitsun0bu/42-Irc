/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intToStr.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 11:31:46 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/10 11:32:48 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/main.hpp"

std::string intToStr (int n)
{
	std::stringstream buff;
	buff << n;
	return (buff.str());
}