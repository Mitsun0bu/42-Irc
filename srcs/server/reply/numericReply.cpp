/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericReply.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:11:42 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 14:59:37 by llethuil         ###   ########lyon.fr   */
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

	if (FD_ISSET(user._socket, &this->_clientFdList.write))
		if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->_clientFdList.write))
		if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + " " + secondParam + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->_clientFdList.write))
		if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string thirdParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + " " + secondParam + " " + thirdParam + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->_clientFdList.write))
		if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ UTILS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

void	Server::initNum(void)
{
	num.ERR_ALREADYREGISTERED		= "462";
	num.ERR_BADCHANMASK				= "476";
	num.ERR_BADCHANNELKEY			= "475";
	num.ERR_BANNEDFROMCHAN			= "474";
	num.ERR_CANNOTSENDTOCHAN		= "404";

	num.ERR_CHANNELISFULL			= "471";
	num.ERR_CHANOPRIVSNEEDED		= "482 ";
	num.ERR_ERRONEUSNICKNAME		= "432";
	num.ERR_INVITEONLYCHAN			= "473";
	num.ERR_NEEDMOREPARAMS			= "461";
	num.ERR_NICKNAMEINUSE			= "433";
	num.ERR_NONICKNAMEGIVEN			= "431";
	num.ERR_NOSUCHCHANNEL			= "403";
	num.ERR_NOSUCHNICK				= "401";
	num.ERR_NORECIPIENT				= "411";
	num.ERR_NOSUCHNICK				= "401";
	num.ERR_NOSUCHSERVER			= "402";
	num.ERR_NOTEXTTOSEND			= "412";
	num.ERR_NOTONCHANNEL			= "442";
	num.ERR_NOTOPLEVEL				= "413";
	num.ERR_NOTREGISTERED			= "451";
	num.ERR_PASSWDMISMATCH			= "464";
	num.ERR_TOOMANYCHANNELS			= "405";
	num.ERR_TOOMANYTARGETS			= "407";
	num.ERR_UNKNOWNCOMMAND			= "421";
	num.ERR_USERONCHANNEL			= "443";
	num.ERR_WILDTOPLEVEL 			= "414";
	num.ERR_USERNOTINCHANNEL		= "441";
	num.ERR_WILDTOPLEVEL			= "414";

	num.MSG_ERR_ALREADYREGISTERED	= " :You may not reregister";
	num.MSG_ERR_BADCHANMASK			= " :Bad Channel Mask";
	num.MSG_ERR_BADCHANNELKEY		= " :Cannot join channel (+k)";
	num.MSG_ERR_BANNEDFROMCHAN		= "";
	num.MSG_ERR_CANNOTSENDTOCHAN	= " :Cannot send to channel";
	num.MSG_ERR_CHANNELISFULL		= "";
	num.MSG_ERR_CHANOPRIVSNEEDED	= " :You're not channel operator";
	num.MSG_ERR_ERRONEUSNICKNAME	= " :Erroneus nickname";
	num.MSG_ERR_INVITEONLYCHAN		= " :Cannot join channel (+i)";
	num.MSG_ERR_NEEDMOREPARAMS		= " :Not enough parameters";
	num.MSG_ERR_NICKNAMEINUSE		= " :Nickname is already in use";
	num.MSG_ERR_NONICKNAMEGIVEN		= " :No nickname given";
	num.MSG_ERR_NOSUCHNICK			= " :No such nick/channel";
	num.MSG_ERR_NOORIGIN			= " :No origin specified";
	num.MSG_ERR_NOSUCHCHANNEL		= " :No such channel";
	num.MSG_ERR_NORECIPIENT			= " :No recipient given";
	num.MSG_ERR_NOSUCHNICK			= " :No such nick/channel";
	num.MSG_ERR_NOSUCHSERVER		= " :No such server";
	num.MSG_ERR_NOTEXTTOSEND		= " :No text to send";
	num.MSG_ERR_NOTONCHANNEL		= " :You're not on that channel";
	num.MSG_ERR_NOTREGISTERED		= " :You have not registered";
	num.MSG_ERR_PASSWDMISMATCH		= " :Password incorrect";
	num.MSG_ERR_TOOMANYCHANNELS		= "";
	num.MSG_MSG_ERR_TOOMANYTARGETS	= " :Duplicate recipients. No message delivered";
	num.MSG_ERR_UNKNOWNCOMMAND		= " :Unknown command";
	num.MSG_ERR_USERONCHANNEL		= " :is already on channel";
	num.MSG_ERR_WILDTOPLEVEL		= " :Wildcard in toplevel domain";
	num.MSG_ERR_USERNOTINCHANNEL	= " :They aren't on that channel";

	num.RPL_AWAY					= "301";
	num.RPL_CREATED					= "003";
	num.RPL_CHANNELMODEIS			= "324";
	num.RPL_ENDOFNAMES				= "366";
	num.RPL_INVITING				= "341";
	num.RPL_LIST					= "322";
	num.RPL_LISTEND					= "323";
	num.RPL_MYINFO					= "004";
	num.RPL_NAMREPLY				= "353";
	num.RPL_NOTOPIC					= "331";
	num.RPL_TOPIC					= "332";
	num.RPL_TOPICWHOTIME			= "333";
	num.RPL_WELCOME					= "001";
	num.RPL_YOURHOST				= "002";

	num.MSG_RPL_AWAY				= "";
	num.MSG_RPL_CREATED				= " :This server was created ";
	num.MSG_RPL_ENDOFNAMES			= " :End of /NAMES list";
	num.MSG_RPL_LISTEND				= " :End of /LIST";
	num.MSG_RPL_MYINFO				= " 127.0.0.1 1 oOr RO";
	num.MSG_RPL_NOTOPIC				= "  :No topic is set";
	num.MSG_RPL_TOPICWHOTIME		= "";
	num.MSG_RPL_WELCOME				= " :Welcome to the 127.0.0.1 Network, ";
	num.MSG_RPL_YOURHOST			= " :Your host is 127.0.0.1, running version 1";
}