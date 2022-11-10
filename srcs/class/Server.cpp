/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/10 17:22:25 by llethuil         ###   ########lyon.fr   */
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

Server::Server(int port, int addressFamily, int socketType, int socketFlag, int socketBlockingMode, int protocol, const char* internetHostAddr) :
	_port(port), _addressFamily(addressFamily), _socketType(socketType), _socketFlag(socketFlag), _socketBlockingMode(socketBlockingMode), _protocol(protocol), _internetHostAddr(internetHostAddr)
{
	std::cout	<< BLUE
				<< "[CONSTRUCTOR] : "
				<< END
				<< "A Server has been created !"
				<< std::endl;

	return ;
}

Server::Server(Server const & src)
{
	std::cout	<< ORANGE
				<< "[COPY CONSTRUCTOR] : "
				<< END
				<< "A Server has been duplicated !"
				<< std::endl;

	*this = src;

	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                         ~~~ OPERATOR OVERLOAD ~~~                          */
/*                                                                            */
/* ************************************************************************** */

Server&	Server::operator=(Server const & src)
{
	this->_port					= src._port;
	this->_addressFamily		= src._addressFamily;
	this->_socketType			= src._socketType;
	this->_socketFlag			= src._socketFlag;
	this->_socketBlockingMode	= src._socketBlockingMode;
	this->_protocol				= src._protocol;
	this->_socket				= src._socket;
	this->_socketAddr			= src._socketAddr;
	this->_internetHostAddr		= src._internetHostAddr;

	return (*this);
}

/* ************************************************************************** */
/*                                                                            */
/*                         ~~~ MEMBER FUNCTIONS ~~~                           */
/*                                                                            */
/* ************************************************************************** */

int		Server::bindSocket(int serverSocket, struct sockaddr_in& socketAddr)
{
	int				bindResult	= 0;
	unsigned int	addrSize	= sizeof(socketAddr);

	bindResult = bind(serverSocket, (const struct sockaddr *)&socketAddr, addrSize);
	if (bindResult == FAILED)
	{
		std::cerr	<< "Error : Server socket cannot bind to address !"
					<< std::endl;

		close(serverSocket);

		return (FAILED);
	}

	return (bindResult);
}

void	Server::setSocketAddr(struct sockaddr_in& socketAddr, const char* internetHostAddr, int port)
{
	bzero(&socketAddr, sizeof(socketAddr));
	socketAddr.sin_addr.s_addr	= inet_addr(internetHostAddr);
	socketAddr.sin_family		= AF_INET;
	socketAddr.sin_port			= htons(port);
}

int		Server::setSocket()
{
	this->_socket = socket(this->_addressFamily, this->_socketType, this->_protocol);

	fcntl(this->_socket, this->_socketFlag, this->_socketBlockingMode);

	this->setSocketAddr(this->_socketAddr, this->_internetHostAddr, this->_port);

	if (this->bindSocket(this->_socket, this->_socketAddr) == FAILED)
		return (FAILED);

	if (listen(this->_socket, SOMAXCONN) == FAILED)
	{
		std::cerr	<< "Error : Server socket cannot listen to the targeted port !"
					<< std::endl;

		return (FAILED);
	}

	return (this->_socket);
}

void	Server::clientFdListInit(void)
{
	this->clientFdList.max			= this->_socket;
	this->clientFdList.t.tv_sec		= 0;
	this->clientFdList.t.tv_usec	= 10;
	FD_ZERO(&this->clientFdList.master);
	FD_ZERO(&this->clientFdList.read);
	FD_ZERO(&this->clientFdList.write);
	FD_SET(this->_socket, &this->clientFdList.master);
}

void	Server::selectClientSocket(void)
{
	this->clientFdList.read	= this->clientFdList.master;
	this->clientFdList.write	= this->clientFdList.master;
	if (select(this->clientFdList.max + 1, &this->clientFdList.read, &this->clientFdList.write, NULL, &this->clientFdList.t) == FAILED)
	{
		perror("select()");
		exit(1);
	}
}

void	Server::searchForData(void)
{
	for (int fd = 0; fd <= this->clientFdList.max; fd++)
	{
		if (FD_ISSET(fd, &this->clientFdList.read))
		{
			if (fd == this->_socket)
				this->acceptNewUser();
			else
				this->handleClientData(&fd);
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                         ~~~ PRIVATE METHODS ~~~                            */
/*                                                                            */
/* ************************************************************************** */

void	Server::acceptNewUser(void)
{
	User		newUser;

	newUser._socket = accept(
								this->_socket,
								(struct sockaddr *)&newUser._socketAddr,
								&newUser._socketAddrSize
							);
	newUser.setIp();

	if (newUser._socket == FAILED)
		perror("accept()");
	else
	{
		FD_SET(newUser._socket, &this->clientFdList.master);

		if (newUser._socket > this->clientFdList.max)
			this->clientFdList.max = newUser._socket;

		// DEBUG
		std::cout	<< "~~~ New connection from "
					<< newUser._ip
					<< " on socket "
					<< newUser._socket
					<< " ~~~"
					<< std::endl;
		this->_users[newUser._socket] = newUser;
	}
}

void	Server::handleClientData(int* currentFd)
{
	char						buffer[256]	= {0};
	int							byteCount	= 0;
	std::vector<std::string>	cmds;

	byteCount = recv(*currentFd, buffer, sizeof buffer, 0);

	if (byteCount <= 0)
	{
		this->printRecvError(byteCount, *currentFd);
		close(*currentFd);
		FD_CLR(*currentFd, &this->clientFdList.master);
	}
	else
	{
		tokenizer(buffer, "\n\r", cmds);
		std::string msg = "001 llethuil :Welcome to the 127.0.0.1 Network, llethuil[!llethuil@127.0.0.1]\r\n";
		send(*currentFd, msg.c_str(), msg.size(), 0);
		for(size_t i = 0; i < cmds.size(); i ++)
		{
			this->setCmdToExecute(cmds[i]);
			this->execCmd(this->_users[*currentFd], cmds[i]);
		}
	}
}

void	Server::printRecvError(int byteCount, int currentFd)
{
	if (byteCount == 0)
			std::cerr << "Socket " << currentFd << " hung up !" << std::endl;
	else
		perror("recv()");
}

void	Server::setCmdToExecute(std::string cmd)
{
	std::string cmdList[15]	= {
								"PASS" , "NICK"   , "USER"   , "PONG"  ,
								"QUIT" , "JOIN"   , "PART"   , "TOPIC" ,
								"NAMES", "LIST"   , "INVITE" , "KICK"  ,
								"MODE" , "PRIVMSG", "NOTICE"
							 };

	this->_nCmd				= 15;

	std::string	currentCmd = cmd.substr(0, cmd.find(' ', 0));

	for (int i = 0; i != this->_nCmd - 1; i++)
	{
		if (currentCmd == cmdList[i])
		{
			this->_cmdToExecute = i;
			return ;
		}
	}
	this->_cmdToExecute = FAILED;
}

void	Server::execCmd(User &user, std::string cmd)
{
	std::vector<std::string>	cmdTokens;
	tokenizer(cmd, " ", cmdTokens);

	switch(this->_cmdToExecute)
	{
		case 0:
			this->execPass(user, cmdTokens);
			break;
		// case 1:
		// !!	This requires that clients send a PASS command before sending the NICK / USER combination. !!
		// case 2:
		// !!	This requires that clients send a PASS command before sending the NICK / USER combination. !!
		case 5 :
			this->execJoin(user, cmdTokens);
		case 7 :
		// 	...
			break;
		// default:
		// 	...
	}
}

void	Server::execPass(User &user, std::vector<std::string> &cmdTokens)
{
	(void)user;
	(void)cmdTokens;

	// if (!user._isAuthenticated)
		// this->numericReply(user._username, 462, "You may not reregister");

	// if (cmdTokens.size() < 2)
	// 	this->numericReply(user._username, 461, cmdTokens[0], "Not enough parameters");

	// if (cmdTokens[2] != "PASS123")
	// 	this->numericReply(user._username, 464, "Password incorrect");

	// if ((cmdTokens.size() < 2 || cmdTokens[2] != "PASS123")
	// 	this->sendError(user, "Authentication failed");
}

void	Server::execJoin(User &user, std::vector<std::string> &cmdTokens)
{
	char						prefix	= '0';
	size_t						i		= 0;
	std::vector<std::string>	names;
	std::vector<std::string>	keys;

	tokenizer(cmdTokens[1], ",", names);
	tokenizer(cmdTokens[2], ",", keys);

	for(i = 0; i < names.size(); i++)
	{
		// CHECK IF CHANNEL NAME IS VALID
		prefix = names[i][0];
		if (prefix != '#')
			this->numericReply(user, ERR_NOSUCHCHANNEL, names[i] + " :No such channel");
		else
		{
	// 		// IF CHANNEL ALREADY EXISTS
	// 		if (this->_channels.find(names[i]) != this->_channels.end())
	// 		{
	// 			// IF USER IS NOT ALREADY IN CHANNEL
	// 			if (this->_channels[names[i]]._members.find(user._socket) == this->_channels[names[i]]._members.end())
	// 			{
	// 				if(this->_channels[names[i]]._requiresKey == true)
	// 				{
	// 					if (this->_channels[names[i]]._key == keys[i])
	// 					{
	// 						// JOIN
	// 							// :WiZ JOIN #Twilight_zone	; WiZ is joining the channel #Twilight_zone
	// 					}
	// 					else
	// 					{
  	// 						// "<client> <channel> :Cannot join channel (+k)"
	// 						this->numericReply(user, ERR_BADCHANNELKEY, names[i] + " :Cannot join channel (+k)");
	// 					}
	// 				}
	// 				else
	// 				{
	// 					// JOIN
	// 						// :WiZ JOIN #Twilight_zone	; WiZ is joining the channel #Twilight_zone
	// 				}
	// 			}
	// 		}
	// 		// IF CHANNEL DOES NOT EXIST
	// 		else
	// 		{
				if (keys[i])
					Channel	newChannel(names[i], keys[i]);
				else
					Channel	newChannel(names[i]);

				// JOIN CHANNEL
				// std::string joinMsg = ":" + user._nickname + "JOIN" + names[i] + " ; " + user._nickname + " is joining the channel" + names[i] + "\r\n";
				std::string	joinMsg	= ":llethuil JOIN" + names[i] + " ; llethuil is joining the channel" + names[i] + "\r\n";
				this->replyToClient(user, joinMsg);

				// ADD NEW CHANNEL TO SERVER CHANNEL MAP
				this->_channels.push_back(newChannel);
	// 		}
		}
	}
}

void	Server::numericReply(User &user, int numReply, std::string msg)
{
	std::string	code		= intToStr(numReply);
	// std::string	finalMsg	= code + " " + user._nickname + " " + msg + "\r\n";
	std::string	finalMsg	= code + " " + "llethuil" + " " + msg + "\r\n";

	std::cout << finalMsg << std::endl;
	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::numericReply(User &user, int numReply, std::string &cmd, std::string msg)
{
	(void)user;
	(void)numReply;
	(void)cmd;
	(void)msg;
}

void	Server::replyToClient(User &user, std::string msg)
{
	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, msg.c_str(), msg.size(), 0) == FAILED)
			perror("send()");
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