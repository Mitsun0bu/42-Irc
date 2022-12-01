/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 00:37:10 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/Channel.hpp"
# include "../../incs/main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                      ~~~ CONSTRUCTORS ~~~                                  */
/*                                                                            */
/* ************************************************************************** */

Channel::Channel(void)
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Channel has been created !"
				<< std::endl;

	return ;
}

Channel::Channel(std::string name) :
	_name(name), _key(), _requiresKey(), _topic(), _topicIsSet(false), _members(), _operators(), _modeKey("-k"), _modeInvite("-i")
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Channel has been created !"
				<< std::endl;

	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                           ~~~ DESTRUCTOR ~~~                               */
/*                                                                            */
/* ************************************************************************** */

Channel::~Channel(void)
{
	std::cout	<< PURPLE
				<< "[DESTRUCTOR] : "
				<< END
				<< "A Channel has been closed."
				<< std::endl;

	return ;
}