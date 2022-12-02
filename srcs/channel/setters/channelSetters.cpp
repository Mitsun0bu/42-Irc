/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelSetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 18:06:56 by llethuil         ###   ########lyon.fr   */
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
	_modeKey		= "";

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

void	Channel::setModeKey(void)
{
	_modeKey = "+k";
}

void	Channel::unsetModeKey(void)
{
	_modeKey = "";
}

void	Channel::setModeInvite(void)
{
	_modeInvite = "+i";
}

void	Channel::unsetModeInvite(void)
{
	_modeInvite = "";
}
