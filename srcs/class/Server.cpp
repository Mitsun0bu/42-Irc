/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/14 11:54:37 by llethuil         ###   ########lyon.fr   */
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
	std::vector<std::string>	cmdTokens;

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
		static int test = 0;
		if (test == 0)
		{
			std::string msg = "001 llethuil :Welcome to the 127.0.0.1 Network, llethuil[!llethuil@127.0.0.1]\r\n";
			send(*currentFd, msg.c_str(), msg.size(), 0);
			test ++;
		}
		for(size_t i = 0; i < cmds.size(); i ++)
		{
			tokenizer(cmds[i], " ", cmdTokens);
			this->execCmd(this->_users[*currentFd], cmdTokens);
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

int		Server::findCmdToExecute(std::string cmd)
{
	const int	nCmd					= 15;
	std::string	cmdList[nCmd]	= {
											"PASS" , "NICK"   , "USER"   , "PONG"  ,
											"QUIT" , "JOIN"   , "PART"   , "TOPIC" ,
											"NAMES", "LIST"   , "INVITE" , "KICK"  ,
											"MODE" , "PRIVMSG", "NOTICE"
							 			  };

	for (int i = 0; i < nCmd; i++)
		if (cmd == cmdList[i])
			return (i);
	return (FAILED);
}

void	Server::execCmd(User &user, std::vector<std::string> &cmdTokens)
{
	int	cmdToExecute = this->findCmdToExecute(cmdTokens[0]);

	switch(cmdToExecute)
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
		case 8 :
			this->execNames(user, cmdTokens);
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
			// IF CHANNEL EXISTS
			if (this->_channels.find(names[i]) != this->_channels.end())
			{
				// IF USER IS NOT ALREADY IN CHANNEL
				if (this->_channels[names[i]]._members.find(user._socket) == this->_channels[names[i]]._members.end())
				{
					// IF THE CHANNEL REQUIRES A KEY
					if(this->_channels[names[i]]._requiresKey == true)
					{
						// CHECK IF KEY IS VALID
						if (this->_channels[names[i]]._key == keys[i])
						{
							// JOIN CHANNEL
								// std::string joinMsg = ":" + user._nickname + " JOIN" + names[i];
							// TOPIC
								// "<client> <channel> :<topic>"
						}
						else
							this->numericReply(user, ERR_BADCHANNELKEY, names[i] + " :Cannot join channel (+k)");
					}
					// IF THE CHANNEL DOES NOT REQUIRE A KEY
					else
					{
						// JOIN CHANNEL
						// std::string joinMsg = ":" + user._nickname + " JOIN" + names[i] + "\r\n";
						// SWITCH TO THE LINE ABOVE WHEN NICK WILL BE SET !!!!!
						std::string	joinMsg = ":llethuil JOIN" + names[i];
						this->replyToClient(user, joinMsg);
						// TOPIC
							// "<client> <channel> :<topic>"
					}
				}
			}
			// IF CHANNEL DOES NOT EXIST
			else
			{
				// INSTANTIATE NEW CHANNEL
				Channel	newChannel(names[i]);

				if (keys.empty() == false && keys[i].length())
					newChannel.setKey(keys[i]);

				// ADD USER TO THE CHANNEL
				newChannel.addMember(user);

				// ADD NEW CHANNEL TO SERVER CHANNEL MAP
				this->addChannel(newChannel, names[i]);

				// JOIN CHANNEL
				// std::string joinMsg = ":" + user._nickname + " JOIN" + names[i];
				// SWITCH TO THE LINE ABOVE WHEN NICK WILL BE SET !!!!!
				std::string	joinMsg = ":llethuil JOIN " + names[i];
				this->replyToClient(user, joinMsg);
			}
		}
	}
}

void	Server::addChannel(Channel &channel, std::string name)
{
	this->_channels[name] = channel;

	return ;
}

void	Server::execNames(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string>	names;
	tokenizer(cmdTokens[1], ",", names);

	char						prefix		= '0';

	for(size_t i = 0; i < names.size(); i++)
	{
		//IF CHANNEL NAME IS INVALID
		prefix = names[i][0];
		if (prefix != '#')
		{
			std::string endMsg = names[i] + " :End of /NAMES list";
			this->numericReply(user, ":127.0.0.1", RPL_ENDOFNAMES, endMsg);
		}
		// IF CHANNEL EXISTS
		if(this->_channels.find(names[i]) != this->_channels.end())
		{
			// std::string namesMsg = intToStr(RPL_NAMREPLY) + " " + user._nickname + " = " + names[i] + " :";
			// SWITCH TO THE LINE ABOVE WHEN NICK WILL BE SET !!!!!
			std::string namesMsg = intToStr(RPL_NAMREPLY) + " llethuil" + " = " + names[i] + " :";

			// ADD CHANNEL MEMBERS' NICKNAME TO MESSAGE
			std::set<int>	members = this->_channels[names[i]]._members;
			for (std::set<int>::iterator fd = members.begin(); fd != members.end(); fd++)
			{
				// DEBUG
				// std::cout << this->_users[*fd]._nickname << std::endl;

				// namesMsg += this->_users[*fd]._nickname;
				// SWITCH TO THE LINE ABOVE WHEN NICK WILL BE SET !!!!!
				namesMsg += "llethuil";
			}
			this->replyToClient(user, namesMsg);
		}
		// std::string endMsg = intToStr(RPL_ENDOFNAMES) + " " + user._nickname + " " + names[i] + " :End of /NAMES list";
		// SWITCH TO THE LINE ABOVE WHEN NICK WILL BE SET !!!!!
		std::string endMsg = intToStr(RPL_ENDOFNAMES) + " llethuil " + names[i] + " :End of /NAMES list";
		this->replyToClient(user, endMsg);
	}
}

void	Server::numericReply(User &user, int numReply, std::string msg)
{
	std::string	code		= intToStr(numReply);
	std::string	finalMsg	= code + " " + user._nickname + " " + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, msg.c_str(), msg.size(), 0) == FAILED)
			perror("send()");

	return ;
}

void	Server::numericReply(User &user, std::string client, int numReply, std::string msg)
{
	std::string	code		= intToStr(numReply);
	std::string	finalMsg	= client + " " + code + " " + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, msg.c_str(), msg.size(), 0) == FAILED)
			perror("send()");

	return ;
}

void	Server::replyToClient(User &user, std::string msg)
{
	msg += "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, msg.c_str(), msg.size(), 0) == FAILED)
			perror("send()");

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