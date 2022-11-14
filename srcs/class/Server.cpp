/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/14 16:42:14 by agirardi         ###   ########lyon.fr   */
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
		tokenizer(bufferStr, "\r\n", cmds);
		
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
	std::string cmdList[15]	= {
								"PASS" , "NICK"   , "USER"   , "PONG"  ,
								"QUIT" , "JOIN"   , "PART"   , "TOPIC" ,
								"NAMES", "LIST"   , "INVITE" , "KICK"  ,
								"MODE" , "PRIVMSG", "NOTICE"
							 };

	for (size_t i = 0; i < 15; i++)
		if (cmd == cmdList[i])
			return (i);
	return (FAILED);
}

void	Server::execCmd(User &user, std::vector<std::string> &cmdTokens)
{
	int	cmdToExecute = this->findCmdToExecute(cmdTokens[0]);

	// if (!user._isAuthenticated && cmdToExecute < 4)
		// ...

	std::cout << "cmdToExecute: " << cmdToExecute << std::endl;

	switch(cmdToExecute)
	{
		case 0:
			this->execPass(user, cmdTokens);
			break;
		case 1:
			this->execNick(user, cmdTokens);
			break;
		case 2:
			this->execUser(user, cmdTokens);
			break;
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
	if (user._isAuthenticated)
		return (this->numericReply(user, num.ERR_ALREADYREGISTERED, num.MSG_ERR_ALREADYREGISTERED));
	else if (cmdTokens.size() < 2)
		return (this->numericReply(user, num.ERR_NEEDMOREPARAMS, cmdTokens[0], num.MSG_ERR_NEEDMOREPARAMS));
	else if (cmdTokens[1] != this->_passwd)
	{
		this->numericReply(user, num.ERR_PASSWDMISMATCH, cmdTokens[1], num.MSG_ERR_PASSWDMISMATCH);
		return (this->sendError(user, "Authentication failed"));
	}
	else
		user._validPasswd = true;
	std::cout << "user._validPasswd: " << user._validPasswd << std::endl;
}

void	Server::execNick(User &user, std::vector<std::string> &cmdTokens)
{
	if (!user._validPasswd)
		return ;
	else if (isNickAvailable(cmdTokens[1]))
		return (this->numericReply(user, num.ERR_NICKNAMEINUSE, cmdTokens[1], num.MSG_ERR_NICKNAMEINUSE));
	else if (!parseNick(cmdTokens[1]))
		return (this->numericReply(user, num.ERR_ERRONEUSNICKNAME, cmdTokens[1], num.MSG_ERR_ERRONEUSNICKNAME));
	else if (cmdTokens.size() < 2)
		return (this->numericReply(user, num.ERR_NONICKNAMEGIVEN, num.MSG_ERR_NONICKNAMEGIVEN));
	else
	{
		user._nickname = cmdTokens[1];
		if (user._isAuthenticated)
		{
			return(cmdReply(user, "NICK", cmdTokens[1]));
		}
		if (!user._username.empty())
			registerUser(user);
	}
}

bool	Server::isNickAvailable(std::string &nickname)
{
	std::map<int, User>::iterator	it;

	for (it = this->_users.begin(); it != this->_users.end(); it++)
			if (nickname == it->second._nickname)
				return (true);
	return (false);
}

bool	Server::parseNick(std::string &nickname)
{
	std::string validSpecialCharset = "-_[]{}\\`|";

	if (nickname.size() > 9)
		return (false);
	
	for (int i = 0; i < nickname.size(); i++)
	{
		if (!isalnum(nickname[i]) && validSpecialCharset.find(nickname[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

void	Server::execUser(User &user, std::vector<std::string> &cmdTokens)
{
	if (!user._validPasswd)
		return ;
	else if (cmdTokens.size() < 4)
		return (this->numericReply(user, num.ERR_NEEDMOREPARAMS, cmdTokens[0], num.MSG_ERR_NEEDMOREPARAMS));
	else if (user._isAuthenticated)
		return (this->numericReply(user, num.ERR_ALREADYREGISTERED, num.MSG_ERR_ALREADYREGISTERED));
	else
	{		
		user._username = cmdTokens[1];
		if (!user._nickname.empty())
			registerUser(user);
	}
}

void	Server::registerUser(User &user)
{
		std::cout << "Registering" << std::endl;
		this->numericReply(user, num.RPL_WELCOME, user._nickname, num.MSG_RPL_WELCOME, user._nickname);
		this->numericReply(user, num.RPL_YOURHOST, user._nickname, num.MSG_RPL_YOURHOST);
		this->numericReply(user, num.RPL_CREATED, user._nickname, num.MSG_RPL_CREATED, _date);
		this->numericReply(user, num.RPL_MYINFO, user._nickname, num.MSG_RPL_MYINFO);
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
		numericReply(user, num.ERR_NEEDMOREPARAMS, cmdTokens[0], num.MSG_ERR_NEEDMOREPARAMS);
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

void	Server::numericReply(User &user, std::string num, std::string msg)
{
	std::string finalMsg = num + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
    if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
      perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
    if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
      perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + " " + secondParam + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
    if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
      perror("send()");
}

void	Server::numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string thirdParam, std::string msg)
{
	std::string finalMsg = num + " " + firstParam + " " + secondParam + " " + thirdParam + msg + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
    if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
    	perror("send()");
}

void    Server::cmdReply(User &user, std::string cmd, std::string param)
{
    std::string finalMsg = user._nickname + " " + cmd + " " + param + "\r\n";

    // std::cout << "REPLY TO CLIENT : " << msg << std::endl;
    // std::cout << "----------------" << std::endl;

    if (FD_ISSET(user._socket, &this->clientFdList.write))
        if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
            perror("send()");

    return ;
}

void				Server::initNum(void)
{
	num.ERR_PASSWDMISMATCH = "464";
	num.MSG_ERR_PASSWDMISMATCH = " :Password incorrect";
	num.ERR_ALREADYREGISTERED = "462";
	num.MSG_ERR_ALREADYREGISTERED = " :You may not reregister";
	num.ERR_NEEDMOREPARAMS	= "461";
	num.MSG_ERR_NEEDMOREPARAMS = " :Not enough parameters";
	num.ERR_NICKNAMEINUSE = "433	";
	num.MSG_ERR_NICKNAMEINUSE = " :Nickname is already in use";
	num.ERR_ERRONEUSNICKNAME = "432";
	num.MSG_ERR_ERRONEUSNICKNAME = " :Erroneus nickname";
	num.ERR_NONICKNAMEGIVEN = "431";
	num.MSG_ERR_NONICKNAMEGIVEN = " :No nickname given";
	num.ERR_NOSUCHCHANNEL	= "403";
	num.MSG_ERR_NOSUCHCHANNEL = " :No such channel";
	num.ERR_TOOMANYCHANNELS	= "405";
	num.MSG_ERR_TOOMANYCHANNELS = "";
	num.ERR_BADCHANNELKEY	= "475";
	num.MSG_ERR_BADCHANNELKEY = " :Cannot join channel (+k)";
	num.ERR_BANNEDFROMCHAN	= "474";
	num.MSG_ERR_BANNEDFROMCHAN = "";
	num.ERR_CHANNELISFULL	= "471";
	num.MSG_ERR_CHANNELISFULL = "";
	num.ERR_INVITEONLYCHAN	= "473";
	num.MSG_ERR_INVITEONLYCHAN = "";
	num.ERR_BADCHANMASK	= "476";
	num.MSG_ERR_BADCHANMASK = "";
	num.RPL_TOPIC	= "332";
	num.MSG_RPL_TOPIC = "";
	num.RPL_TOPICWHOTIME	= "333";
	num.MSG_RPL_TOPICWHOTIME = "";
	num.RPL_NAMREPLY	= "353";
	num.RPL_ENDOFNAMES	= "366";
	num.MSG_RPL_ENDOFNAMES = " :End of /NAMES list";
	num.RPL_MYINFO	= "004";
	num.MSG_RPL_MYINFO = " 127.0.0.1 1 oOr RO";
	num.RPL_CREATED	= "003";
	num.MSG_RPL_CREATED = " :This server was created ";
	num.RPL_YOURHOST	= "002";
	num.MSG_RPL_YOURHOST = " :Your host is 127.0.0.1, running version 1";
	num.RPL_WELCOME	= "001";
	num.MSG_RPL_WELCOME = " :Welcome to the 127.0.0.1 Network, ";
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


// :127.0.0.1 462 llethuil :You may not reregister
// :127.0.0.1 461 PASS :Not enough parameters
// :127.0.0.1 464 :Password Incorrect

// :127.0.0.1 433 llethuil :Nickname is already in use.
// :127.0.0.1 432 llethuil! :Erroneous Nickname
// :127.0.0.1 431 :No nickname given

// :127.0.0.1 001 llethuil :Welcome to the 127.0.0.1 Network, llethuil
// :127.0.0.1 002 llethuil :Your host is 127.0.0.1, running version 1
// :127.0.0.1 003 llethuil :This server was created Tue Nov 30 2011 at 11:11:25
// :127.0.0.1 004 llethuil 127.0.0.1 1 oOr <available cmodes>





  

