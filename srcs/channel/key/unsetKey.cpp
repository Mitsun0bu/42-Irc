/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetKey.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:52:09 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/29 10:12:42 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/Channel.hpp"
# include "../../../incs/main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

void	Channel::unsetKey()
{
	_key.clear();
	_requiresKey	= false;
	_modeKey		= "-k";

	return ;
}