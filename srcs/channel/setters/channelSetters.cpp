/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelSetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 15:40:01 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/Channel.hpp"

void	Channel::setName(std::string name)
{
	_name = name;
}

void	Channel::setKey(std::string key)
{
	_key			= key;
	_requiresKey	= true;
	_modeKey		= "+k";

	return ;
}

void	Channel::unsetKey()
{
	_key.clear();
	_requiresKey	= false;
	_modeKey		= "-k";

	return ;
}

void	Channel::setRequiresKey(bool isRequired)
{
	_requiresKey = isRequired;
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void	Channel::setTopicIsSet(bool isTopicSet)
{
	_topicIsSet = isTopicSet;
}

void	Channel::setMembers(std::set<int> members)
{
	_members = members;
}

void	Channel::setBannedMembers(std::set<int> bannedMembers)
{
	_bannedMembers = bannedMembers;
}

void	Channel::setOperators(std::set<int> operators)
{
	_operators = operators;
}

void	Channel::setModeKey(std::string mode)
{
	_modeKey = mode;
}

void	Channel::setModeInvite(std::string mode)
{
	_modeInvite = mode;
}
