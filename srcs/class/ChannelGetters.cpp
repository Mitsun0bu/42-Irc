/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelGetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 03:22:07 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/Channel.hpp"

std::string		Channel::getName(void) const
{
	return _name;
}

std::string		Channel::getKey(void) const
{
	return _key;
}

bool					Channel::getRequiresKey(void) const
{
	return _requiresKey;
}

std::string		Channel::getTopic(void) const
{
	return _topic;
}

bool					Channel::getTopicIsSet(void) const
{
	return _topicIsSet;
}

std::set<int>	Channel::getMembers(void) const
{
	return _members;
}

std::set<int>	Channel::getBannedMembers(void) const
{
	return _bannedMembers;
}

std::set<int>	Channel::getOperators(void) const
{
	return _operators;
}

std::string		Channel::getMode(void) const
{
	return _mode;
}
