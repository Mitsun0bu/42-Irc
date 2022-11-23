/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/23 01:31:19 by agirardi         ###   ########lyon.fr   */
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
				this->handleClientData(fd);
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

void	Server::handleClientData(int &currentFd)
{
	char						buffer[256]	= {0};
	int							byteCount	= 0;
	std::string					bufferStr;
	std::vector<std::string>	cmds;
	std::vector<std::string>	cmdTokens;


	byteCount = recv(currentFd, buffer, 256, 0);
	if (byteCount <= 0)
	{

		this->printRecvError(byteCount, currentFd);
		if (_users.find(currentFd) != _users.end())
			logoutUser(_users[currentFd]);
		close(currentFd);
		FD_CLR(currentFd, &this->clientFdList.master);
	}
	else
	{
		bufferStr = buffer;
		tokenizer(bufferStr, "\r\n", cmds);

		for(size_t i = 0; i < cmds.size(); i ++)
		{
			tokenizer(cmds[i], " ", cmdTokens);
			this->handleCmd(this->_users[currentFd], cmdTokens);
		}
	}
}

void	Server::printRecvError(int byteCount, int currentFd)
{
	if (byteCount == 0)
	{
		std::cerr << "Socket " << currentFd << " hung up !" << std::endl;
	}
	else
		perror("recv()");
}

int		Server::findCmdToExecute(std::string &cmd)
{
	const int	nCmd	= 16;
	std::string cmdList[nCmd]	= {
								"PASS" 	, "CAP"		, "NICK"   	, "USER" 	,
								"PING" 	, "QUIT"	, "JOIN"  	, "PART" 	, 
								"TOPIC"	, "NAMES"	, "LIST"  	, "INVITE",	
								"KICK" 	, "MODE"	, "PRIVMSG"	,"NOTICE"
							 };

	for (size_t i = 0; i < nCmd; i++)
		if (cmd == cmdList[i])
			return (i);
	return (FAILED);
}

void	Server::handleCmd(User &user, std::vector<std::string> &cmdTokens)
{
	int	cmdToExecute = this->findCmdToExecute(cmdTokens[0]);

	if (!user._isAuthenticated && cmdToExecute > 4)
		return(this->numericReply(user, num.ERR_NOTREGISTERED, cmdTokens[0], num.MSG_ERR_NOTREGISTERED));

	switch(cmdToExecute)
	{
		case PASS:
			this->handlePass(user, cmdTokens);
			break;
		case CAP:
			break;
		case NICK:
			this->handleNick(user, cmdTokens);
			break;
		case USER:
			this->handleUser(user, cmdTokens);
			break;
		case PING:
			this->handlePing(user, cmdTokens);
			break;
		case QUIT:
			this->handleQuit(user, cmdTokens);
			break;
		case JOIN:
			this->handleJoin(user, cmdTokens);
			break;
		case NAMES:
			this->handleNames(user, cmdTokens);
			break;
		default:
			this->numericReply(user, num.ERR_UNKNOWNCOMMAND, cmdTokens[0], num.MSG_ERR_UNKNOWNCOMMAND);
	}
}

void	Server::handlePing(User &user, std::vector<std::string> &cmdTokens)
{
	if (cmdTokens.size() < 2)
		this->numericReply(user, num.ERR_NEEDMOREPARAMS, cmdTokens[0], num.MSG_ERR_NEEDMOREPARAMS);
	
	std::string host = _internetHostAddr;
	cmdReply(user, "PONG", host + " " + cmdTokens[1]);
}

void	Server::logoutUser(User &user)
{
	std::map<std::string, Channel>::iterator it = _channels.begin();

	while (it != _channels.end())
	{
		if (it->second._operators.find(user._socket) != it->second._operators.end())
			it->second._operators.erase(user._socket);
		if (it->second._members.find(user._socket) != it->second._members.end())
		{
			it->second._members.erase(user._socket);
			if (it->second._members.size() == 0)
			{
				std::map<std::string, Channel>::iterator toErase = it;
				++it;
				_channels.erase(toErase);
			}
			else
				++it;
		}
	}
	this->_users.erase(user._socket);
}

void	Server::handleQuit(User &user, std::vector<std::string> &cmdTokens)
{
	std::string msg = "Quit: ";

	for (size_t i = 1; i < cmdTokens.size(); i++)
		msg.append(cmdTokens[i]);
	cmdReply(user, "QUIT", msg);
}

void	Server::handlePass(User &user, std::vector<std::string> &cmdTokens)
{
	if (user._isAuthenticated)
		this->numericReply(user, num.ERR_ALREADYREGISTERED, num.MSG_ERR_ALREADYREGISTERED);
	else if (cmdTokens.size() < 2)
		this->numericReply(user, num.ERR_NEEDMOREPARAMS, cmdTokens[0], num.MSG_ERR_NEEDMOREPARAMS);
	else if (cmdTokens[1] != this->_passwd)
	{
		this->numericReply(user, num.ERR_PASSWDMISMATCH, cmdTokens[1], num.MSG_ERR_PASSWDMISMATCH);
		this->errorReply(user, "Authentication failed");
	}
	else
		user._validPasswd = true;
}

void	Server::handleNick(User &user, std::vector<std::string> &cmdTokens)
{
	if (!user._validPasswd)
		return ;
	else if (!isNickAvailable(cmdTokens[1]))
		this->numericReply(user, num.ERR_NICKNAMEINUSE, cmdTokens[1], num.MSG_ERR_NICKNAMEINUSE);
	else if (!parseNick(cmdTokens[1]))
		this->numericReply(user, num.ERR_ERRONEUSNICKNAME, cmdTokens[1], num.MSG_ERR_ERRONEUSNICKNAME);
	else if (cmdTokens.size() < 2)
		this->numericReply(user, num.ERR_NONICKNAMEGIVEN, num.MSG_ERR_NONICKNAMEGIVEN);
	else
	{
		if (user._isAuthenticated)
			return(this->cmdReply(user, "NICK", cmdTokens[1]));
		user._nickname = cmdTokens[1];
		if (!user._username.empty())
			registerUser(user);
	}
}

bool	Server::isNickAvailable(std::string &nickname)
{
	std::map<int, User>::iterator	it;

	for (it = this->_users.begin(); it != this->_users.end(); it++)
			if (nickname == it->second._nickname)
				return (false);
	return (true);
}

bool	Server::parseNick(std::string &nickname)
{
	std::string validSpecialCharset = "-_[]{}\\`|";

	if (nickname.size() > 9)
		return (false);
	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (!isalnum(nickname[i]) && validSpecialCharset.find(nickname[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

void	Server::handleUser(User &user, std::vector<std::string> &cmdTokens)
{
	if (!user._validPasswd)
		return ;
	else if (cmdTokens.size() < 4)
		this->numericReply(user, num.ERR_NEEDMOREPARAMS, cmdTokens[0], num.MSG_ERR_NEEDMOREPARAMS);
	else if (user._isAuthenticated)
		this->numericReply(user, num.ERR_ALREADYREGISTERED, num.MSG_ERR_ALREADYREGISTERED);
	else
	{
		user._username = cmdTokens[1];
		if (!user._nickname.empty())
			registerUser(user);
	}
}

void	Server::registerUser(User &user)
{
	user._isAuthenticated = true;
	this->numericReply(user, num.RPL_WELCOME, user._nickname, num.MSG_RPL_WELCOME, user._nickname);
	this->numericReply(user, num.RPL_YOURHOST, user._nickname, num.MSG_RPL_YOURHOST);
	this->numericReply(user, num.RPL_CREATED, user._nickname, num.MSG_RPL_CREATED, _date);
	this->numericReply(user, num.RPL_MYINFO, user._nickname, num.MSG_RPL_MYINFO);
}

void	Server::handleJoin(User &user, std::vector<std::string> &cmdTokens)
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
			this->numericReply(user, num.ERR_NOSUCHCHANNEL, names[i] + " :No such channel");
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
							this->numericReply(user, num.ERR_BADCHANNELKEY, names[i] + " :Cannot join channel (+k)");
					}
					// IF THE CHANNEL DOES NOT REQUIRE A KEY
					else
					{
						// JOIN CHANNEL
						this->cmdReply(user, "JOIN", names[i]);
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
				this->cmdReply(user, "JOIN", names[i]);

				// CALL NAMES FUNCTION
				this->handleNames(user, cmdTokens);
			}
		}
	}
}

void	Server::addChannel(Channel &channel, std::string name)
{
	this->_channels[name] = channel;

	return ;
}

void	Server::handleNames(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string>	names;
	tokenizer(cmdTokens[1], ",", names);
	char						prefix		= '0';

	for(size_t i = 0; i < names.size(); i++)
	{
		//IF CHANNEL NAME IS INVALID
		prefix = names[i][0];
		if (prefix != '#')
			this->numericReply(user, num.RPL_ENDOFNAMES, user._nickname + " " + names[i], num.MSG_RPL_ENDOFNAMES);
		// IF CHANNEL EXISTS
		if(this->_channels.find(names[i]) != this->_channels.end())
		{
			std::string namesMsg = " = " + names[i] + " :";
			// ADD CHANNEL MEMBERS' NICKNAME TO MESSAGE
			std::set<int>	members = this->_channels[names[i]]._members;
			for (std::set<int>::iterator fd = members.begin(); fd != members.end(); fd++)
				namesMsg += this->_users[*fd]._nickname;
			this->numericReply(user, num.RPL_NAMREPLY, namesMsg);
		}
		this->numericReply(user, num.RPL_ENDOFNAMES, user._nickname + " " + names[i], num.MSG_RPL_ENDOFNAMES);
	}
}

void	Server::errorReply(User &user, std::string reason)
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

void	Server::cmdReply(User &user, std::string cmd, std::string firstParam)	
{
    std::string finalMsg = ":" + user._nickname + " " + cmd + " " + firstParam + "\r\n";

    if (FD_ISSET(user._socket, &this->clientFdList.write))
        if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
            perror("send()");
}

void	Server::sendCmd(User &from, std::string cmd, User &target, std::string msg)
{
    std::string finalMsg = ":" + from._nickname + " " + cmd + " " + target._nickname + " " + msg + "\r\n";

    if (FD_ISSET(target._socket, &this->clientFdList.write))
        if (send(target._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
            perror("send()");
}

void	Server::initNum(void)
{
	num.ERR_ALREADYREGISTERED	= "462";
	num.ERR_BADCHANMASK				= "476";
	num.ERR_BADCHANNELKEY			= "475";
	num.ERR_BANNEDFROMCHAN		= "474";
	num.ERR_CANNOTSENDTOCHAN 	= "404";

	num.ERR_CHANNELISFULL			= "471";
	num.ERR_ERRONEUSNICKNAME		= "432";
	num.ERR_INVITEONLYCHAN			= "473";
	num.ERR_NEEDMOREPARAMS			= "461";
	num.ERR_NICKNAMEINUSE			= "433";
	num.ERR_NONICKNAMEGIVEN			= "431";
	num.ERR_NOSUCHCHANNEL			= "403";
	num.ERR_NORECIPIENT = "411";
	num.ERR_NOSUCHNICK = "401";
	num.ERR_NOSUCHSERVER = "402";
	num.ERR_NOTEXTTOSEND = "412";
	num.ERR_NOTONCHANNEL			= "442";
	num.ERR_NOTOPLEVEL = "413";
	num.ERR_NOTREGISTERED			= "451";
	num.ERR_PASSWDMISMATCH			= "464";
	num.ERR_TOOMANYCHANNELS			= "405";
	num.ERR_TOOMANYTARGETS = "407";
	num.ERR_UNKNOWNCOMMAND			= "421";
	num.ERR_WILDTOPLEVEL = "414";

	num.MSG_ERR_ALREADYREGISTERED	= " :You may not reregister";
	num.MSG_ERR_BADCHANMASK			= " :Bad Channel Mask";
	num.MSG_ERR_BADCHANNELKEY		= " :Cannot join channel (+k)";
	num.MSG_ERR_BANNEDFROMCHAN		= "";
	num.MSG_ERR_CANNOTSENDTOCHAN = " :Cannot send to channel";
	num.MSG_ERR_CHANNELISFULL		= "";
	num.MSG_ERR_ERRONEUSNICKNAME	= " :Erroneus nickname";
	num.MSG_ERR_INVITEONLYCHAN		= "";
	num.MSG_ERR_NEEDMOREPARAMS		= " :Not enough parameters";
	num.MSG_ERR_NICKNAMEINUSE		= " :Nickname is already in use";
	num.MSG_ERR_NONICKNAMEGIVEN		= " :No nickname given";
	num.MSG_ERR_NOORIGIN = " :No origin specified";
	num.MSG_ERR_NOSUCHCHANNEL		= " :No such channel";
	num.MSG_ERR_NORECIPIENT = " :No recipient given";
	num.MSG_ERR_NOSUCHNICK = " :No such nick/channel";
	num.MSG_ERR_NOSUCHSERVER = " :No such server";
	num.MSG_ERR_NOTEXTTOSEND = " :No text to send";
	num.MSG_ERR_NOTONCHANNEL		= " :You're not on that channel";
	num.MSG_ERR_NOTREGISTERED		= " :You have not registered";
	num.MSG_ERR_PASSWDMISMATCH		= " :Password incorrect";
	num.MSG_ERR_TOOMANYCHANNELS		= "";
	num.MSG_MSG_ERR_TOOMANYTARGETS = " :Duplicate recipients. No message delivered";
	num.MSG_ERR_UNKNOWNCOMMAND		= " :Unknown command";
	num.MSG_ERR_WILDTOPLEVEL = " :Wildcard in toplevel domain";

	num.RPL_AWAY = "301";
	num.RPL_CREATED					= "003";
	num.RPL_ENDOFNAMES				= "366";
	num.RPL_MYINFO					= "004";
	num.RPL_NAMREPLY				= "353";
	num.RPL_NOTOPIC					= "331";
	num.RPL_TOPIC					= "332";
	num.RPL_TOPICWHOTIME			= "333";
	num.RPL_WELCOME					= "001";
	num.RPL_YOURHOST				= "002";

	num.MSG_RPL_AWAY = "";
	num.MSG_RPL_CREATED				= " :This server was created ";
	num.MSG_RPL_ENDOFNAMES			= " :End of /NAMES list";
	num.MSG_RPL_MYINFO				= " 127.0.0.1 1 oOr RO";
	num.MSG_RPL_NOTOPIC				= "  :No topic is set";
	num.MSG_RPL_TOPICWHOTIME		= "";
	num.MSG_RPL_WELCOME				= " :Welcome to the 127.0.0.1 Network, ";
	num.MSG_RPL_YOURHOST			= " :Your host is 127.0.0.1, running version 1";

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