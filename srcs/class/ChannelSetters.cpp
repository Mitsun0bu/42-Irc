/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelSetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:25:31 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 03:27:27 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/Channel.hpp"

void					Channel::setName(std::string name)
{
	_name = name;
}

void					Channel::setKey(std::string key)
{
	_key			= key;
	_requiresKey	= true;
	_mode			= "+k";
}

void					Channel::setRequiresKey(bool isRequired)
{
	_requiresKey = isRequired;
}

void					Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void					Channel::setTopicIsSet(bool isTopicSet)
{
	_topicIsSet = isTopicSet;
}

void					Channel::setMembers(std::set<int> members)
{
	_members = members;
}

void					Channel::setBannedMembers(std::set<int> bannedMembers)
{
	_bannedMembers = bannedMembers;
}

void					Channel::setOperators(std::set<int> operators)
{
	_operators = operators;
}

void					Channel::setMode(std::string mode)
{
	_mode = mode;
}
