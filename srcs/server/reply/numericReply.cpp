/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericReply.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:11:42 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 00:30:11 by agirardi         ###   ########lyon.fr   */
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

void	Server::numericReply(User &user, std::string num, std::string msg)
{
	std::string finalMsg = num + msg + "\r\n";

	if (FD_ISSET(user.getSocket(), &this->_clientFdList.write))
		if (send(user.getSocket(), finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + msg + "\r\n";

	if (FD_ISSET(user.getSocket(), &this->_clientFdList.write))
		if (send(user.getSocket(), finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + " " + secondParam + msg + "\r\n";

	if (FD_ISSET(user.getSocket(), &this->_clientFdList.write))
		if (send(user.getSocket(), finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string thirdParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + " " + secondParam + " " + thirdParam + msg + "\r\n";

	if (FD_ISSET(user.getSocket(), &this->_clientFdList.write))
		if (send(user.getSocket(), finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

void	Server::initNum(void)
{
	_num.ERR_ALREADYREGISTERED		= "462";
	_num.ERR_BADCHANMASK			= "476";
	_num.ERR_BADCHANNELKEY			= "475";
	_num.ERR_BANNEDFROMCHAN			= "474";
	_num.ERR_CANNOTSENDTOCHAN		= "404";

	_num.ERR_CHANNELISFULL			= "471";
	_num.ERR_CHANOPRIVSNEEDED		= "482 ";
	_num.ERR_ERRONEUSNICKNAME		= "432";
	_num.ERR_INVITEONLYCHAN			= "473";
	_num.ERR_NEEDMOREPARAMS			= "461";
	_num.ERR_NICKNAMEINUSE			= "433";
	_num.ERR_NONICKNAMEGIVEN		= "431";
	_num.ERR_NOSUCHCHANNEL			= "403";
	_num.ERR_NOSUCHNICK				= "401";
	_num.ERR_NORECIPIENT			= "411";
	_num.ERR_NOSUCHNICK				= "401";
	_num.ERR_NOSUCHSERVER			= "402";
	_num.ERR_NOTEXTTOSEND			= "412";
	_num.ERR_NOTONCHANNEL			= "442";
	_num.ERR_NOTOPLEVEL				= "413";
	_num.ERR_NOTREGISTERED			= "451";
	_num.ERR_PASSWDMISMATCH			= "464";
	_num.ERR_TOOMANYCHANNELS		= "405";
	_num.ERR_TOOMANYTARGETS			= "407";
	_num.ERR_UNKNOWNCOMMAND			= "421";
	_num.ERR_USERONCHANNEL			= "443";
	_num.ERR_WILDTOPLEVEL 			= "414";
	_num.ERR_USERNOTINCHANNEL		= "441";
	_num.ERR_WILDTOPLEVEL			= "414";

	_num.MSG_ERR_ALREADYREGISTERED	= " :You may not reregister";
	_num.MSG_ERR_BADCHANMASK		= " :Bad Channel Mask";
	_num.MSG_ERR_BADCHANNELKEY		= " :Cannot join channel (+k)";
	_num.MSG_ERR_BANNEDFROMCHAN		= "";
	_num.MSG_ERR_CANNOTSENDTOCHAN	= " :Cannot send to channel";
	_num.MSG_ERR_CHANNELISFULL		= "";
	_num.MSG_ERR_CHANOPRIVSNEEDED	= " :You're not channel operator";
	_num.MSG_ERR_ERRONEUSNICKNAME	= " :Erroneus nickname";
	_num.MSG_ERR_INVITEONLYCHAN		= " :Cannot join channel (+i)";
	_num.MSG_ERR_NEEDMOREPARAMS		= " :Not enough parameters";
	_num.MSG_ERR_NICKNAMEINUSE		= " :Nickname is already in use";
	_num.MSG_ERR_NONICKNAMEGIVEN	= " :No nickname given";
	_num.MSG_ERR_NOSUCHNICK			= " :No such nick/channel";
	_num.MSG_ERR_NOORIGIN			= " :No origin specified";
	_num.MSG_ERR_NOSUCHCHANNEL		= " :No such channel";
	_num.MSG_ERR_NORECIPIENT		= " :No recipient given";
	_num.MSG_ERR_NOSUCHNICK			= " :No such nick/channel";
	_num.MSG_ERR_NOSUCHSERVER		= " :No such server";
	_num.MSG_ERR_NOTEXTTOSEND		= " :No text to send";
	_num.MSG_ERR_NOTONCHANNEL		= " :You're not on that channel";
	_num.MSG_ERR_NOTREGISTERED		= " :You have not registered";
	_num.MSG_ERR_PASSWDMISMATCH		= " :Password incorrect";
	_num.MSG_ERR_TOOMANYCHANNELS	= "";
	_num.MSG_MSG_ERR_TOOMANYTARGETS	= " :Duplicate recipients. No message delivered";
	_num.MSG_ERR_UNKNOWNCOMMAND		= " :Unknown command";
	_num.MSG_ERR_USERONCHANNEL		= " :is already on channel";
	_num.MSG_ERR_WILDTOPLEVEL		= " :Wildcard in toplevel domain";
	_num.MSG_ERR_USERNOTINCHANNEL	= " :They aren't on that channel";

	_num.RPL_AWAY					= "301";
	_num.RPL_CREATED				= "003";
	_num.RPL_CHANNELMODEIS			= "324";
	_num.RPL_ENDOFNAMES				= "366";
	_num.RPL_INVITING				= "341";
	_num.RPL_LIST					= "322";
	_num.RPL_LISTEND				= "323";
	_num.RPL_MYINFO					= "004";
	_num.RPL_NAMREPLY				= "353";
	_num.RPL_NOTOPIC				= "331";
	_num.RPL_TOPIC					= "332";
	_num.RPL_TOPICWHOTIME			= "333";
	_num.RPL_WELCOME				= "001";
	_num.RPL_YOURHOST				= "002";

	_num.MSG_RPL_AWAY				= "";
	_num.MSG_RPL_CREATED			= " :This server was created ";
	_num.MSG_RPL_ENDOFNAMES			= " :End of /NAMES list";
	_num.MSG_RPL_LISTEND			= " :End of /LIST";
	_num.MSG_RPL_MYINFO				= " 127.0.0.1 1 oOr RO";
	_num.MSG_RPL_NOTOPIC			= "  :No topic is set";
	_num.MSG_RPL_TOPICWHOTIME		= "";
	_num.MSG_RPL_WELCOME			= " :Welcome to the 127.0.0.1 Network, ";
	_num.MSG_RPL_YOURHOST			= " :Your host is 127.0.0.1, running version 1";
}