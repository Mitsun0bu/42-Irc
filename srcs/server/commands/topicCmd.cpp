/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCmd.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:44:47 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 17:51:48 by llethuil         ###   ########lyon.fr   */
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
/*                               ~~~ FUNCTION ~~~                             */
/*                                                                            */
/* ************************************************************************** */

void	Server::topicCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::string	channelName	= cmdTokens[1];
	std::string	topic		= "";

	if (cmdTokens.size() > 2)
		topic = cmdTokens[2];

	if (user._locations.find(channelName) == user._locations.end())
	{
		std::string	notInChannelMsg = " " + user._nickname + " " + channelName;
		numericReply(user, _num.ERR_NOTONCHANNEL, notInChannelMsg, _num.MSG_ERR_NOTONCHANNEL);
		return ;
	}

	if (topic.empty() == true)
		replyTopic(user, channelName);
	else if (topic.empty() == false && topic == "::")
	{
		clearTopic(channelName);
		cmdReply(user, "TOPIC", channelName + " :");
	}
	else if (topic.empty() == false)
	{
		clearTopic(channelName);
		setTopic(channelName, topic);
		cmdReply(user, "TOPIC", channelName + " :" + _channels[channelName]._topic);
	}
	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

void	Server::clearTopic(std::string channelName)
{
	_channels[channelName]._topic.clear();
	_channels[channelName]._topicIsSet = false;
	return ;
}

void	Server::replyTopic(User& user, std::string channelName)
{
	std::string topicMsg = " " + user._nickname + " " + channelName;

	if (_channels[channelName]._topicIsSet == true)
	{
		topicMsg += " " + _channels[channelName]._topic;
		numericReply(user, _num.RPL_TOPIC, topicMsg);
	}
	else
		numericReply(user, _num.RPL_NOTOPIC, topicMsg, _num.MSG_RPL_NOTOPIC);
	return ;
}

void	Server::setTopic(std::string channelName, std::string topic)
{
	_channels[channelName]._topic		= topic.erase(0, 1);
	_channels[channelName]._topicIsSet	= true;
	return ;
}