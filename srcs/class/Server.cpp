/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/09 15:54:56 by agirardi         ###   ########lyon.fr   */
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

void	Server::selectClientSocket(t_fdList *clientFdList)
{
	clientFdList->read	= clientFdList->master;
	clientFdList->write	= clientFdList->master;
	if (select(clientFdList->max + 1, &clientFdList->read, &clientFdList->write, NULL, &clientFdList->t) == FAILED)
	{
		perror("select()");
		exit(1);
	}
}

void	Server::searchForData(t_fdList *clientFdList)
{
	for (int fd = 0; fd <= clientFdList->max; fd++)
	{
		if (FD_ISSET(fd, &clientFdList->read))
		{
			if (fd == this->_socket)
				this->acceptNewUser(clientFdList);
			else
				this->handleClientData(clientFdList, &fd);
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                         ~~~ PRIVATE METHODS ~~~                            */
/*                                                                            */
/* ************************************************************************** */

void	Server::acceptNewUser(t_fdList *clientFdList)
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
		FD_SET(newUser._socket, &clientFdList->master);

		if (newUser._socket > clientFdList->max)
			clientFdList->max = newUser._socket;

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

void	Server::handleClientData(t_fdList *clientFdList, int* currentFd)
{
	// char						buffer[256]	= {0};
	std::string					buffer;
	int							byteCount	= 0;
	std::vector<std::string>	cmds;

	byteCount = recv(*currentFd, const_cast<char*>(buffer.c_str()), sizeof buffer, 0);

	if (byteCount <= 0)
	{
		this->printRecvError(byteCount, *currentFd);
		close(*currentFd);
		FD_CLR(*currentFd, &clientFdList->master);
	}
	else
	{
		tokenizer(buffer, "\n", cmds);
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
	std::string cmdList[17]	= {
								"PASS" , 	"NICK", "USER",
								"PONG" , "QUIT" , "JOIN",
								"PART"  , "TOPIC", "NAMES", "LIST",
								"INVITE", "KICK", "MODE" , "PRIVMSG",
								"NOTICE"
							 };

	this->_nCmd				= 17;

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

		case 7 :
			this->execJoin(user, cmdTokens);
			break;
		// default:
		// 	...
	}
}

void	Server::execPass(User &user, std::vector<std::string> &cmdTokens)
{
	(void)user;
	
	// if (!user._isAuthenticated)
		// this->numericReply(user._username, 462, "You may not reregister");

	// if (cmdTokens.size() < 2)
	// 	this->numericReply(user._username, 461, cmdTokens[0], "Not enough parameters");

	// if (cmdTokens[2] != "PASS123")
	// 	this->numericReply(user._username, 464, "Password incorrect");

	if ((cmdTokens.size() < 2 || cmdTokens[2] != "PASS123")
		this->sendError(user, "Authentication failed");
	
	
	
		
	
}

void	Server::execJoin(User &user, std::vector<std::string> &cmdTokens)
{
	(void)user;
	/*
	The server receiving the command checks whether
	or not the client can join the given channel,
	and processes the request.
	Servers MUST process the parameters of this command as lists
	on incoming commands from clients,
	with the first <key> being used for the first <channel>,
	the second <key> being used for the second <channel>, etc.
	*/

	if (cmdTokens.size() < 2)
	{
		// numericReply(ERR_NEEDMOREPARAMS, user._socket, this->_users, &cmdTokens[0]);
		return ;
	}
}



void	Server::sendClientData(t_fdList *clientFdList, int* currentFd, char* buffer, int byteCount)
{
	for(int fd = 0; fd <= clientFdList->max; fd++)
	{
		if (FD_ISSET(fd, &clientFdList->master))
		{
			if (fd != this->_socket && fd != *currentFd)
			{
				if (send(fd, buffer, byteCount, 0) == -1)
					perror("send()");
			}
		}
	}
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