/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/01 00:30:44 by agirardi         ###   ########lyon.fr   */
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
/*                      ~~~ CONSTRUCTORS ~~~                                  */
/*                                                                            */
/* ************************************************************************** */

Server::Server(void)
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Server has been created !"
				<< std::endl;

	return ;
}

Server::Server(int port, std::string passwd, int addressFamily, int socketType, int socketFlag, int socketBlockingMode, int protocol, const char* internetHostAddr) :
	_port(port), _passwd(passwd), _addressFamily(addressFamily), _socketType(socketType), _socketFlag(socketFlag), _socketBlockingMode(socketBlockingMode), _protocol(protocol), _internetHostAddr(internetHostAddr)
{
	std::time_t t = std::time(0);

	this->_date = ctime(&t);
	this->_date.pop_back();
	initNum();

	std::string cmdNames[NCMD]	= {
						"PASS" 	, "CAP"		, "NICK"   	, "USER" 	,
						"PING" 	, "QUIT"	, "JOIN"  	, "PART" 	,
						"TOPIC"	, "NAMES"	, "LIST"  	, "INVITE",
						"KICK" 	, "MODE"	, "PRIVMSG"	, "NOTICE"
						};

	for (int i = 0; i < NCMD; i++)
		_cmdMap.insert(std::pair<int,std::string>(i, cmdNames[i]));

	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Server has been created !"
				<< std::endl;
	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                           ~~~ DESTRUCTOR ~~~                               */
/*                                                                            */
/* ************************************************************************** */

Server::~Server(void)
{
	std::cout	<< PURPLE
				<< "[DESTRUCTOR] : "
				<< END
				<< "A Server has been closed."
				<< std::endl;

	close(this->_socket);

	return ;
}