/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelGetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 01:18:14 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/Channel.hpp"

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

std::set<int>	&Channel::getMembers(void)
{
	return _members;
}

std::set<int>	&Channel::getAllowedMembers(void)
{
	return _allowedMembers;
}

std::set<int>	&Channel::getBannedMembers(void)
{
	return _bannedMembers;
}

std::set<int>	&Channel::getOperators(void)
{
	return _operators;
}

std::string		Channel::getModeKey(void) const
{
	return _modeKey;
}

std::string		Channel::getModeInvite(void) const
{
	return _modeInvite;
}
