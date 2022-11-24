/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:46:23 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/24 19:30:33 by llethuil         ###   ########lyon.fr   */
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
	std::vector<std::string>	cmds;
	std::vector<std::string>	cmdTokens;
	User &currentUser = _users[currentFd];

	byteCount = recv(currentFd, buffer, 256, 0);
	currentUser._cmd += buffer;
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
		if (currentUser._cmd[currentUser._cmd.length() - 1] != '\n')
			return ;
		tokenizer(currentUser._cmd, "\r\n", cmds);

		for(size_t i = 0; i < cmds.size(); i ++)
		{
			tokenizer(cmds[i], " ", cmdTokens);
			this->handleCmd(currentUser, cmdTokens);
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
			this->handleJoinCmd(user, cmdTokens);
			break;
		case PART:
			this->handlePartCmd(user, cmdTokens);
			break;
		case TOPIC:
			this->handleTopicCmd(user, cmdTokens);
			break;
		case NAMES:
			this->handleNamesCmd(user, cmdTokens);
			break;
		case LIST:
			this->handleListCmd(user, cmdTokens);
			break;
		case INVITE:
			this->handleInviteCmd(user, cmdTokens);
			break;
		case MODE:
			this->handleChannelMode(user, cmdTokens);
			break;
		case PRIVMSG:
			this->handlePrivmsg(user, cmdTokens);
			break;
		default:
			this->numericReply(user, num.ERR_UNKNOWNCOMMAND, cmdTokens[0], num.MSG_ERR_UNKNOWNCOMMAND);
	}

	user._cmd.clear();
}

void	Server::handlePrivmsg(User &user, std::vector<std::string> &cmdTokens)
{
	// forcer les ':' avant le msg ?
	// Parsing de channelName -> '#' possible a l'intérieur du nom ?
	// _users peuvent ils être away ?

	std::vector<std::string> targets;
	std::string msg;

	for (size_t i = 2; i < cmdTokens.size(); i++)
		msg.append(cmdTokens[i]);

	tokenizer(cmdTokens[1], ",", targets);
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].find("#") != std::string::npos)
			sendMsgToChannel(user, targets[i], msg);
		else
			sendMsgToUser(user, targets[i], msg);
	}
}

void	Server::sendMsgToUser(User &sender, std::string &target, std::string &msg)
{
	int targetSocket = getUserSocket(target);

	if (targetSocket == FAILED)
		numericReply(sender, num.ERR_NOSUCHNICK, target, num.MSG_ERR_NOSUCHNICK);
	else
	{
		sendCmdToUser(sender, "PRIVMSG", _users[targetSocket], msg);
	}
}

void	Server::sendMsgToChannel(User &sender, std::string &target, std::string &msg)
{
	std::string channelName = target.substr(target.find("#"), std::string::npos);

	if (_channels.find(channelName) == _channels.end())
		return(numericReply(sender, num.ERR_NOSUCHNICK, target, num.MSG_ERR_NOSUCHNICK));

	Channel &channel = _channels[channelName];
	if	(!parseTargetPrefix(target) || !checkUserPermissions(sender, channel))
		return(numericReply(sender, num.ERR_CANNOTSENDTOCHAN, target, num.MSG_ERR_CANNOTSENDTOCHAN));

	if (target[0] == '@')
		sendCmdToChannel(sender, "PRIVMSG", channel._operators, channelName, msg);
	else
		sendCmdToChannel(sender, "PRIVMSG", channel._members, channelName, msg);
}

bool	Server::checkUserPermissions(User &user, Channel &channel)
{
	if (channel._bannedMembers.find(user._socket) != channel._bannedMembers.end() ||
			channel._members.find(user._socket) == channel._members.end())
		return (false);
	return (true);
}

bool	Server::parseTargetPrefix(const std::string &target)
{
	size_t pos = target.find("#");

	if (pos > 1)
		return (false);
	if (pos == 1 && target[0] != '@')
		return (false);
	return (true);
}

int		Server::getUserSocket(std::string &nickname)
{
	std::map<int, User>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second._nickname == nickname)
			return (it->second._socket);
	}
	return (FAILED);
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

void	Server::handleJoinCmd(User &user, std::vector<std::string> &cmdTokens)
{
	size_t						i		= 0;
	std::vector<std::string>	channelNames;
	std::vector<std::string>	channelKeys;
	std::string					topicMsg;

	tokenizer(cmdTokens[1], ",", channelNames);
	if(cmdTokens.size() > 2)
		tokenizer(cmdTokens[2], ",", channelKeys);

	for (i = 0; i < channelNames.size(); i++)
	{
		if (checkChannelName(channelNames[i]) == FAILED)
		{
			numericReply(user, num.ERR_NOSUCHCHANNEL, channelNames[i], num.MSG_ERR_NOSUCHCHANNEL);
			continue ;
		}
		// IF CHANNEL ALREADY EXISTS
		else if (_channels.find(channelNames[i]) != _channels.end())
		{
			// IF USER IS ALREADY IN CHANNEL
			if (_channels[channelNames[i]]._members.find(user._socket) != _channels[channelNames[i]]._members.end())
				return ;
			// IF THE CHANNEL REQUIRES A KEY AND KEY IS WRONG
			if(_channels[channelNames[i]]._requiresKey == true
			&& (channelKeys.size() == 0 || _channels[channelNames[i]]._key != channelKeys[i]))
			{
				numericReply(user, num.ERR_BADCHANNELKEY, channelNames[i], num.MSG_ERR_BADCHANNELKEY);
				return ;
			}
			_channels[channelNames[i]]._members.insert(user._socket);
		}
		// IF CHANNEL DOES NOT EXIST
		else
		{
			Channel	newChannel(channelNames[i]);

			user._mode = "+o";

			if (channelKeys.empty() == false && channelKeys[i].length())
				newChannel.setKey(channelKeys[i]);
			newChannel.addMember(user);
			newChannel.addOperator(user);

			addChannel(newChannel, channelNames[i]);
		}

		user.addLocation(channelNames[i]);
		cmdReply(user, "JOIN", channelNames[i]);
		sendCmdToChannel(user, "JOIN", _channels[channelNames[i]]._members, channelNames[i], "");
		replyTopic(user, channelNames[i]);
		handleNamesCmd(user, cmdTokens);
	}
}

int		Server::checkChannelName(std::string channelName)
{
	char prefix = channelName[0];
	if (prefix != '#')
		return (FAILED);

	// Parsing de channelName -> ‘#’ possible a l’intérieur du nom ?


	return (SUCCESS);
}

bool	Server::channelExists(std::string channelName)
{
	if (_channels.find(channelName) != _channels.end())
		return (true);
	else
		return (false);
}

void	Server::addChannel(Channel &channel, std::string &channelName)
{
	_channels[channelName] = channel;
	return ;
}

void	Server::deleteChannel(std::string &channelName)
{
	std::map<std::string, Channel>::iterator	channelIterator;

	for(channelIterator = _channels.begin(); channelIterator != _channels.end(); ++channelIterator)
	{
		if (channelIterator->first == channelName)
		{
			_channels.erase(channelName);
			return ;
		}
	}

	return ;
}

void	Server::handlePartCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string>	channelNames;
	std::string					reason = "";
	std::map<std::string, Channel>::iterator it;

	tokenizer(cmdTokens[1], ",", channelNames);
	if(cmdTokens.size() == 3)
		reason = cmdTokens[2];

	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::cout << "TURN #" << i << std::endl;

		if (_channels.find(channelNames[i]) == _channels.end())
		{
			numericReply(user, num.ERR_NOSUCHCHANNEL, channelNames[i], num.MSG_ERR_NOSUCHCHANNEL);
			continue;
		}

		for(it = _channels.begin(); it != _channels.end(); ++it)
		{
			// HANDLE OPERATORS
			// if (it->second._operators.find(user._socket) != it->second._operators.end())
			// 	it->second._operators.erase(user._socket);

			if (it->first == channelNames[i])
			{
				if (user._locations.find(channelNames[i]) == user._locations.end())
				{
					std::string	notInChannelMsg = " " + user._nickname + " " + channelNames[i];
					numericReply(user, num.ERR_NOTONCHANNEL, notInChannelMsg, num.MSG_ERR_NOTONCHANNEL);
					continue ;
				}

				it->second._members.erase(user._socket);
				user._locations.erase(channelNames[i]);
				cmdReply(user, "PART", channelNames[i] + " " + reason);

				if (it->second._members.size() == 0)
				{
					std::map<std::string, Channel>::iterator toErase;
					toErase = it;
					_channels.erase(toErase);
					if (_channels.size() == 0)
					{
						_channels.clear();
						return ;
					}
				}
			}
		}
	}
}

void	Server::handleTopicCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::string	channelName	= cmdTokens[1];
	std::string	topic		= "";

	if (cmdTokens.size() > 2)
		topic = cmdTokens[2];

	if (user._locations.find(channelName) == user._locations.end())
	{
		std::string	notInChannelMsg = " " + user._nickname + " " + channelName;
		numericReply(user, num.ERR_NOTONCHANNEL, notInChannelMsg, num.MSG_ERR_NOTONCHANNEL);
		return ;
	}

	if (topic.empty() == true)
		replyTopic(user, channelName);
	else if (topic.empty() == false && topic == "::")
	{
		clearTopic(channelName);
		cmdReply(user, "TOPIC", channelName + " :");
	}
	else if (topic.empty() == false)
	{
		clearTopic(channelName);
		setTopic(channelName, topic);
		cmdReply(user, "TOPIC", channelName + " :" + _channels[channelName]._topic);
	}
	return ;
}

void	Server::replyTopic(User& user, std::string channelName)
{
	std::string topicMsg = " " + user._nickname + " " + channelName;

	if (_channels[channelName]._topicIsSet == true)
	{
		topicMsg += " " + _channels[channelName]._topic;
		numericReply(user, num.RPL_TOPIC, topicMsg);
	}
	else
		numericReply(user, num.RPL_NOTOPIC, topicMsg, num.MSG_RPL_NOTOPIC);
	return ;
}

void	Server::clearTopic(std::string channelName)
{
	_channels[channelName]._topic.clear();
	_channels[channelName]._topicIsSet = false;
	return ;
}

void	Server::setTopic(std::string channelName, std::string topic)
{
	_channels[channelName]._topic		= topic.erase(0, 1);
	_channels[channelName]._topicIsSet	= true;
	return ;
}

void	Server::handleNamesCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::string					nick = user._nickname;
	std::vector<std::string>	channelNames;
	tokenizer(cmdTokens[1], ",", channelNames);


	for(size_t i = 0; i < channelNames.size(); i++)
	{
		//IF CHANNEL NAME IS INVALID
		if (checkChannelName(channelNames[i]) == FAILED)
		{
			this->numericReply(user, num.RPL_ENDOFNAMES, nick + " " + channelNames[i], num.MSG_RPL_ENDOFNAMES);
			continue ;
		}
		// IF CHANNEL EXISTS
		if(this->_channels.find(channelNames[i]) != this->_channels.end())
		{
			std::string namesMsg = " = " + channelNames[i] + " :";
			// ADD CHANNEL MEMBERS' NICKNAME TO MESSAGE
			std::set<int>	members = this->_channels[channelNames[i]]._members;
			for (std::set<int>::iterator fd = members.begin(); fd != members.end(); fd++)
			{
				if (_channels[channelNames[i]]._operators.find(*fd) != _channels[channelNames[i]]._operators.end())
					namesMsg += "@" + this->_users[*fd]._nickname;
				else
					namesMsg += this->_users[*fd]._nickname;
				namesMsg += " ";
			}
			std::cout << "namesMsg: " << namesMsg << std::endl;
			this->numericReply(user, num.RPL_NAMREPLY, namesMsg);
		}
		this->numericReply(user, num.RPL_ENDOFNAMES, nick + " " + channelNames[i], num.MSG_RPL_ENDOFNAMES);
	}
}

void	Server::handleListCmd(User &user, std::vector<std::string> &cmdTokens)
{
	std::vector<std::string>	channelNames;

	if (cmdTokens.size() == 1)
	{
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			std::string	memberCount	= intToStr(it->second._members.size());
			std::string	listMsg		= " " + user._nickname + " " + it->second._name + " " + memberCount + " :" + it->second._topic;
			numericReply(user, num.RPL_LIST, listMsg);
		}
	}
	else
	{
		tokenizer(cmdTokens[1], ",", channelNames);
		for(size_t i = 0; i < channelNames.size(); i++)
		{
			std::string	memberCount	= intToStr(_channels[channelNames[i]]._members.size());
			std::string	topic		= _channels[channelNames[i]]._topic;
			std::string	listMsg		= " " + user._nickname + " " + channelNames[i] + " " + memberCount + " :" + topic;
			numericReply(user, num.RPL_LIST, listMsg);
		}
	}
	numericReply(user, num.RPL_LISTEND, "", num.MSG_RPL_LISTEND);
}

void	Server::handleInviteCmd(User &user, std::vector<std::string> &cmdTokens)
{

	// DEBUG BLOCK
	std::cout << "~~~ HANDLE INVITE - START ~~~" << std::endl;

	for(size_t i = 0; i < cmdTokens.size(); i++)
		std::cout << cmdTokens[i] << std::endl;
	// DEBUG BLOCK

	if (cmdTokens.size() != 3)
		return ;

	std::string	userToInvite	= cmdTokens[1];
	std::string	channelName		= cmdTokens[2];

	// IF CHANNEL DOES NOT EXIST
	if (channelExists(channelName) == false)
	{
		numericReply(user, num.ERR_NOSUCHCHANNEL, channelName, num.MSG_ERR_NOSUCHCHANNEL);
		return ;
	}

	// IF INVITING USER IS NOT A CHANNEL MEMBER
	if (_channels[channelName].isMember(user._socket) == false)
	{
		std::string	notInChannelMsg = " " + user._nickname + " " + channelName;
		numericReply(user, num.ERR_NOTONCHANNEL, notInChannelMsg, num.MSG_ERR_NOTONCHANNEL);
		return ;
	}

	// CHECK INVITE-ONLY MODE

	// Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric.
	// They SHOULD reject it when the channel has invite-only mode set,
	// and the user is not a channel operator.

	// IF USER TO INVITE IS ALREADY A CHANNEL MEMBER
	if (_channels[channelName].isMember(getUserSocket(userToInvite)) == true)
	{
		// ERR_USERONCHANNEL
		// "<client> <nick> <channel> :is already on channel"
		std::string	inChannelMsg = " " + userToInvite + " " + channelName;
		numericReply(user, num.ERR_USERONCHANNEL, inChannelMsg, num.MSG_ERR_USERONCHANNEL);
		return ;
	}

	// When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer,
	// and an INVITE message, with the issuer as <source>, to the target user.
	// Other channel members SHOULD NOT be notified.

	numericReply(user, num.RPL_INVITING, user._nickname + " " + userToInvite + " " + channelName);
	sendInvitation(userToInvite, channelName);

	// DEBUG BLOCK
	std::cout << "~~~ HANDLE INVITE - END ~~~" << std::endl;
	// DEBUG BLOCK
}

void	Server::sendInvitation(std::string userToInvite, std::string channelName)
{
	std::string	invitationMsg = "INVITE " + userToInvite + " " + channelName + "\r\n";
	int	socket = getUserSocket(userToInvite);

	if (FD_ISSET(socket, &this->clientFdList.write))
		if (send(socket, invitationMsg.c_str(), invitationMsg.size(), 0) == FAILED)
			perror("send()");
}


void	Server::handleChannelMode(User& user, std::vector<std::string> &cmdTokens)
{
	for(size_t i = 0; i < cmdTokens.size(); i++)
		std::cout << cmdTokens[i] << std::endl;

	std::string	target = cmdTokens[1];

	// IF TARGET IS NOT A VALID/EXISTING CHANNEL
	if (handleChannelModeError(user, target) == FAILED)
		return ;

	// IF NO MODESTRING IS GIVEN
	if (cmdTokens.size() == 2)
		numericReply(user, num.RPL_CHANNELMODEIS, " " + target + " " + _channels[target]._mode);

	if (cmdTokens.size() >= 3)
		handleModeString(user, cmdTokens, _channels[target]);
}

int		Server::handleChannelModeError(User& user, std::string& channelName)
{
	// CHECK IF MODE ARGUMENT IS A CHANNEL
	if (checkChannelName(channelName) == FAILED)
		return (FAILED);

	// CHECK IF MODE ARGUMENT IS A CHANNEL THAT EXISTS IN THE SERVER
	if (_channels.find(channelName) == _channels.end())
	{
		numericReply(user, num.ERR_NOSUCHCHANNEL, channelName, num.MSG_ERR_NOSUCHCHANNEL);
		return (FAILED);
	}

	return (SUCCESS);
}

void	Server::handleModeString(User &user, std::vector<std::string> &cmdTokens, Channel& channel)
{
	std::string					modestring = cmdTokens[2];

	std::vector<std::string>	modearguments;
	if (cmdTokens.size() > 3)
		tokenizer(cmdTokens[3], " ", modearguments);

	// CHECK IF THE USER IS AN OPERATOR
	if (user.isOperator(channel._operators) == false)
	{
		numericReply(user, num.ERR_CHANOPRIVSNEEDED, channel._name, num.MSG_ERR_CHANOPRIVSNEEDED);
		return ;
	}

	// CHECK IF THE MODESTRING IS VALID
	if (modestring.size() != 2 && (modestring[0] != '-' || modestring[0] != '+'))
		return ;

	// HANDLE CHANNEL KEY MODE
	if (modestring[1] == 'k')
	{
		if ((modestring[0] == '-' && modearguments.size() != 0)
		||	(modestring[0] == '+' && modearguments.size() == 0))
			return ;

		if (modestring[0] == '-' && modearguments.size() == 0)
			channel.unsetKey();
		else if (modestring[0] == '+' && modearguments.size() != 0)
			channel.setKey(modearguments[0]);

		numericReply(user, num.RPL_CHANNELMODEIS, " " + channel._name + " " + channel._mode);
	}

	// HANDLE CHANNEL OPERATOR MODE
	if (modestring[1] == 'o' && modearguments[0].length() != 0)
	{
		if (channel.isMember(getUserSocket(modearguments[0])) == false)
			return ;
		if (modestring[0] == '-')
		{
			channel.removeOperator(getUserSocket(modearguments[0]));
			if (channel._operators.size() == 0)
				deleteChannel(channel._name);
		}
		else if (modestring[0] == '+')
			channel.addOperator(getUserSocket(modearguments[0]));
		cmdReply(user, "MODE", channel._name + " " + modestring + " " + modearguments[0]);
	}

	return ;
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

void	Server::cmdReply(User &user, std::string cmd, std::string param)
{
	std::string finalMsg = ":" + user._nickname + " " + cmd + " " + param + "\r\n";

	if (FD_ISSET(user._socket, &this->clientFdList.write))
		if (send(user._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::sendCmdToUser(User &from, std::string cmd, User &target, std::string msg)
{
	std::string finalMsg = ":" + from._nickname + " " + cmd + " " + target._nickname + " " + msg + "\r\n";

	if (FD_ISSET(target._socket, &this->clientFdList.write))
		if (send(target._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
			perror("send()");
}

void	Server::sendCmdToChannel(User &from, std::string cmd, std::set<int> &targets, std::string channel, std::string msg)
{
	std::string finalMsg = ":" + from._nickname + " " + cmd + " " + channel + " " + msg + "\r\n";
	std::set<int>::iterator it;

	for (it = targets.begin(); it != targets.end(); ++it)
	{
		if (_users[*it]._socket != from._socket && FD_ISSET(_users[*it]._socket, &this->clientFdList.write))
			if (send(_users[*it]._socket, finalMsg.c_str(), finalMsg.size(), 0) == FAILED)
				perror("send()");
	}
}

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

	num.MSG_ERR_ALREADYREGISTERED	= " :You may not reregister";
	num.MSG_ERR_BADCHANMASK			= " :Bad Channel Mask";
	num.MSG_ERR_BADCHANNELKEY		= " :Cannot join channel (+k)";
	num.MSG_ERR_BANNEDFROMCHAN		= "";
	num.MSG_ERR_CANNOTSENDTOCHAN	= " :Cannot send to channel";
	num.MSG_ERR_CHANNELISFULL		= "";
	num.MSG_ERR_CHANOPRIVSNEEDED	= " :You're not channel operator";
	num.MSG_ERR_ERRONEUSNICKNAME	= " :Erroneus nickname";
	num.MSG_ERR_INVITEONLYCHAN		= "";
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