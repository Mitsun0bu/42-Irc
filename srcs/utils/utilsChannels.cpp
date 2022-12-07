/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsChannels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:56:47 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/07 17:00:38 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../incs/Server.hpp"
# include "../../incs/main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ FUNCTIONS ~~~                            */
/*                                                                            */
/* ************************************************************************** */

void	Server::addChannel(Channel &channel, std::string &channelName)
{
	_channels[channelName] = channel;
	return ;
}

bool	Server::channelExists(std::string channelName)
{
	if (_channels.find(channelName) != _channels.end())
		return (true);
	else
		return (false);
}

int		Server::checkChannelName(std::string channelName)
{
	char prefix = channelName[0];

	// CHECK IF CHANNEL NAME STARTS WITH '#' PREFIX
	if (prefix != '#')
		return (FAILED);

	// CHECK IF CHANNEL NAME CONTAINS '#' CHARACTER
	for(size_t i = 1; i < channelName.size(); i++)
		if (channelName[i] == '#')
			return (FAILED);

	return (SUCCESS);
}

void	Server::deleteChannel(const std::string channelName)
{
	std::map<std::string, Channel>::iterator	channelIterator;

	for(channelIterator = _channels.begin(); channelIterator != _channels.end(); ++channelIterator)
	{
		if (channelIterator->first == channelName)
		{
			_channels.erase(channelName);
			return ;
		}
	}

	return ;
}

void	Server::removeUserFromChannel(User &user, Channel &channel)
{
	if (channel.getOperators().find(user.getSocket()) != channel.getOperators().end())
		channel.getOperators().erase(user.getSocket());
	if (channel.getMembers().find(user.getSocket()) != channel.getMembers().end())
		channel.getMembers().erase(user.getSocket());
	if (channel.getMembers().size() == 0)
		_channels.erase(channel.getName());
}