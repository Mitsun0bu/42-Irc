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

	if (cmdTokens.size() < 2)
		return(this->numericReply(user, _num.ERR_NEEDMOREPARAMS, cmdTokens[0], _num.MSG_ERR_NEEDMOREPARAMS));

	if (cmdTokens.size() > 2)
		topic = cmdTokens[2];

	if (user.getLocations().find(channelName) == user.getLocations().end())
	{
		std::string	notInChannelMsg = " " + user.getNickname() + " " + channelName;
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
		cmdReply(user, "TOPIC", channelName + " :" + _channels[channelName].getTopic());
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
	_channels[channelName].getTopic().clear();
	_channels[channelName].setTopicIsSet(false);
	return ;
}

void	Server::replyTopic(User& user, std::string channelName)
{
	std::string topicMsg = " " + user.getNickname() + " " + channelName;

	if (_channels[channelName].getTopicIsSet() == true)
	{
		topicMsg += " " + _channels[channelName].getTopic();
		numericReply(user, _num.RPL_TOPIC, topicMsg);
	}
	else
		numericReply(user, _num.RPL_NOTOPIC, topicMsg, _num.MSG_RPL_NOTOPIC);
	return ;
}

void	Server::setTopic(std::string channelName, std::string topic)
{
	_channels[channelName].setTopic(topic.erase(0, 1));
	_channels[channelName].setTopicIsSet(true);
	return ;
}
