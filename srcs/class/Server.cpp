/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/11 14:11:23 by agirardi         ###   ########lyon.fr   */
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
	std::string			bufferStr;
	std::vector<std::string>	cmds;
	std::vector<std::string>	cmdTokens;


	byteCount = recv(*currentFd, buffer, 256, 0);
	if (byteCount <= 0)
	{
		this->printRecvError(byteCount, *currentFd);
		close(*currentFd);
		FD_CLR(*currentFd, &this->clientFdList.master);
	}
	else
	{
		bufferStr = buffer;
		tokenizer(bufferStr, "\n", cmds);
		
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

int	Server::findCmdToExecute(std::string &cmd)
{
	const int nCmd = 15;
	std::string cmdList[nCmd]	= {
								"PASS" , "NICK"   , "USER"   , "PONG"  ,
								"QUIT" , "JOIN"   , "PART"   , "TOPIC" ,
								"NAMES", "LIST"   , "INVITE" , "KICK"  ,
								"MODE" , "PRIVMSG", "NOTICE"
							 };

	for (size_t i = 0; i < nCmd; i++)
		if (cmd == cmdList[i])
			return (i);
	return (FAILED);
}

void	Server::execCmd(User &user, std::vector<std::string> &cmdTokens)
{
	int	cmdToExecute = this->findCmdToExecute(cmdTokens[0]);

	// Bouger tokenizer dans la for loop de handleClientData()
	// Supprimer currentCmd dans setCmdToExecute()

	// this->_cmdToExecute utile ?





	// if (!user._isAuthenticated && this->_cmdToExecute < 3)
	// 	...
	// else
	// 	switch
		


	switch(cmdToExecute)
	{
		case 0:
			this->execPass(user, cmdTokens);
			break;
		// case 1:
		// !!	This requires that clients send a PASS command before sending the NICK / USER combination. !!
		// case 2:
		// !!	This requires that clients send a PASS command before sending the NICK / USER combination. !!

		case 7 :
		// 	...
		case 6 :
			this->execJoin(user, cmdTokens);
			break;
		// default:
		// 	...
	}
}

void	Server::execPass(User &user, std::vector<std::string> &cmdTokens)
{
	// std::cout << "passwd: '" << this->_passwd << "'" << std::endl; // debug
	// std::cout << "cmdTokens[1]: '" << cmdTokens[1] << "'" << std::endl << std::endl; // debug

	if (user._isAuthenticated)
		return this->numericReply(user, 462, ":You may not reregister");
	else if (cmdTokens.size() < 2)
		return this->numericReply(user, 461, cmdTokens[0], ":Not enough parameters");
	else if (cmdTokens[1] != this->_passwd)
	{	
		this->numericReply(user, 464, ":Password incorrect");
		return this->sendError(user, "Authentication failed");
	}
	else
		user._validPasswd = true;


}

void	Server::execNick(User &user, std::vector<std::string> &cmdTokens)
{
	if (!user._validPasswd)
		return ;
	else if (searchForUser(cmdTokens[1]))
		return this->numericReply(user, 433, cmdTokens[1], ":Nickname is already in use");
	

}

bool	Server::searchForUser(std::string nickname)
{
	std::map<int, User>::iterator	it;

	for (it = this->_users.begin(); it != this->_users.end(); it++)
	{
			std::cout << it->first << ':' << it->second._nickname << std::endl; // debug
			if (nickname == it->second._nickname)
				return true;
	}
	return false;
}

bool	Server::parseNick(std::string nickname)
{
	// std::string validSpecialCharset = "-_[]{}\`|";

	if (nickname.size() > 9)
		return false;
	
	return true;
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
		numericReply(user, ERR_NEEDMOREPARAMS, cmdTokens[0], ":Not enough parameters");
		return ;
	}
}

void	Server::sendError(User &user, std::string reason)
{
	std::string cmd = "Error :" + reason + "\r\n";
	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, cmd.c_str(), cmd.size(), 0) == FAILED)
			perror("send");
}

void	Server::numericReply(User &user, int numReply, std::string msg)
{
	std::string code        = intToStr(numReply);
	// std::string  finalMsg    = code + " " + user._nickname + " " + msg + "\r\n";
	std::string finalMsg    = code + " " + "alex" + " " + msg + "\r\n";

    std::cout << finalMsg << std::endl;
    if (FD_ISSET(user._socket, &this->clientFdList.write))
        if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
            perror("send");
}

void	Server::numericReply(User &user, int numReply, std::string &cmd, std::string msg)
{
	(void)user;
	(void)numReply;
	(void)cmd;
	(void)msg;
}

void	Server::sendClientData(int* currentFd, char* buffer, int byteCount)
{
	for(int fd = 0; fd <= this->clientFdList.max; fd++)
	{
		if (FD_ISSET(fd, &this->clientFdList.master))
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