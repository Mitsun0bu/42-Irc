/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsChannel.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:56:47 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 18:58:06 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "../../../incs/Server.hpp"
# include "../../../incs/main.hpp"

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
	if (prefix != '#')
		return (FAILED);

	// Parsing de channelName -> ‘#’ possible a l’intérieur du nom ?

	return (SUCCESS);
}

void	Server::deleteChannel(std::string &channelName)
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
	if (channel._operators.find(user._socket) != channel._operators.end())
		channel._operators.erase(user._socket);
	if (channel._members.find(user._socket) != channel._members.end())
		channel._members.erase(user._socket);
	if (channel._members.size() == 0)
		_channels.erase(channel._name);
}