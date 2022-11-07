/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/07 16:34:43 by llethuil         ###   ########lyon.fr   */
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
	clientFdList->read = clientFdList->master;
	if (select(clientFdList->max + 1, &clientFdList->read, NULL, NULL, &clientFdList->t) == FAILED)
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

void	Server::acceptNewUser(t_fdList *clientFdList)
{
	User		newUser;

	newUser._socket = accept(
								this->_socket,
								(struct sockaddr *)&newUser._socketAddr,
								&newUser._socketAddrSize
							);

	if (newUser._socket == FAILED)
		perror("accept()");
	else
	{
		FD_SET(newUser._socket, &clientFdList->master);

		if (newUser._socket > clientFdList->max)
			clientFdList->max = newUser._socket;

		// DEBUG
		std::cout	<< "~~~ New connection from "
					<< newUser.getIp()
					<< " on socket "
					<< newUser._socket
					<< " ~~~"
					<< std::endl;

		this->_users[newUser._socket] = newUser;
	}
}

void	Server::handleClientData(t_fdList *clientFdList, int* currentFd)
{
	char						buffer[256]	= {0};
	int							byteCount	= 0;

	std::vector<std::string>	cmds;

	byteCount = recv(*currentFd, buffer, sizeof buffer, 0);
	if (byteCount <= 0)
	{
		this->printRecvError(byteCount, *currentFd);
		close(*currentFd);
		FD_CLR(*currentFd, &clientFdList->master);
	}
	else
	{
		tokenizeBuffer(buffer, "\n", cmds);
		std::string msg = "001 llethuil :Welcome to the 127.0.0.1 Network, llethuil[!llethuil@127.0.0.1]\r\n";
		send(*currentFd, msg.c_str(), msg.size(), 0);
		for(size_t i = 0; i < cmds.size(); i ++)
		{
			this->setCmdToExecute(cmds[i]);
			this->execCmd(this->_users[*currentFd], cmds[i]);
		}

		// this->sendClientData(clientFdList, currentFd, buffer, byteCount);
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
	std::string	currentCmd = cmd.substr(0, cmd.find(' ', 0));

	for (int i = 0; i != static_cast<int>(this->_cmdList.size()); i++)
	{
		if (currentCmd == this->_cmdList[i])
		{
			this->_cmdToExecute = i;
			return ;
		}
	}
	this->_cmdToExecute = FAILED;
}

void	Server::execCmd(User &user, std::string cmd)
{
	switch(this->_cmdToExecute)
	{
		// case 0:
		// 	...
		// case 1:
		// 	...
		// default:
		// 	...
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

	// DEBUG
	std::cout << "BUFF = " << buffer << std::endl;
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