/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:59 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/08 14:33:03 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include "main.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ CLASS ~~~                                */
/*                                                                            */
/* ************************************************************************** */

class	User;
class	Channel;

class	Server
{
	public :
		/* constructors													*/
										Server(void);
										Server(
													int			port,
													std::string	passwd,
													int			addressFamily,
													int			socketType,
													int			socketFlag,
													int			socketBlockingMode,
													int			protocol,
													const char*	internetHostAddr
											  );

		/* public attributes											*/
		int								_port;
		std::string						_passwd;
		int								_addressFamily;
		int								_socketType;
		int								_socketFlag;
		int								_socketBlockingMode;
		int								_protocol;
		const char*						_internetHostAddr;
		int								_socket;
		int								_nCmd;
		struct sockaddr_in				_socketAddr;
		t_num							_num;
		t_fdList						_clientFdList;
		std::string						_date;
		std::map<int, User>				_users;
		std::map<std::string, Channel>	_channels;
		std::map<int, std::string>		_cmdMap;

		/* member functions												*/
		int								bindSocket(int serverSocket, struct sockaddr_in& socketAddr);
		void							setSocketAddr(struct sockaddr_in& socketAddr, int port);
		int								setSocket(void);
		void							clientFdListInit(void);
		void							selectClientSocket(void);
		void							searchForData(void);



		/* destructor													*/
										~Server(void);

	private :

		/* private methods												*/
		void							acceptNewUser();

		void							handleClientData(int &currentFd);
		void							printRecvError(int currentFd);
		void							printRecvError(int currentFd, int byteCount);

		int								findCmdToExecute(std::string &cmd);

		void							handleCmd(User &user, std::vector<std::string> &cmd);



		/* COMMANDS */

		/* INVITE */
		void							inviteCmd(User &user, std::vector<std::string> &cmdTokens);
		int								handleInviteError(User &user, std::string channelName);
		void							sendInvitation(std::string userInviting, std::string userToInvite, std::string channelName);

		/* JOIN */
		void							joinCmd(User &user, std::vector<std::string> &cmdTokens);
		int								joinExistingChannel(User& user, int i, std::vector<std::string> channelNames, std::vector<std::string> channelKeys);
		void							joinNewChannel(User& user, int i, std::vector<std::string> channelNames, std::vector<std::string> channelKeys);
		void							doJoin(User& user, std::vector<std::string> &cmdTokens, int i, std::vector<std::string> channelNames);


		/* KICK */
		void							kickCmd(User &user, std::vector<std::string> &cmdTokens);

		/* LIST */
		void							listCmd(User &user, std::vector<std::string> &cmdTokens);

		/* MODE */
		void							modeCmd(User &user, std::vector<std::string> &cmdTokens);
		int								findErrorInModeCmd(User& user, std::string& channelName);
		void							handleModeString(User &user, std::vector<std::string> &cmdTokens, Channel& channel);
		int								findErrorInModeString(User& user, Channel& channel, std::string modestring);
		int								handleKeyMode(Channel& channel, std::string modestring, std::vector<std::string> modearguments);
		void							handleInviteMode(Channel& channel, std::string modestring);
		int								handleOperatorMode(User& user, Channel& channel, std::string modestring, std::vector<std::string> modearguments);

		/* NAMES */
		void							namesCmd(User &user, std::vector<std::string> &cmdTokens);

		/* NICK */
		void							nickCmd(User &user, std::vector<std::string> &cmdTokens);
		bool							parseNick(std::string &nickname);
		void							notifyAllChannels(User &user, std::string &nickname);

		/* PART */
		void							partCmd(User &user, std::vector<std::string> &cmdTokens);
		int								leaveChannel(User &user, Channel& channel, std::string channelName, std::string reason);

		/* PASS */
		void							passCmd(User &user, std::vector<std::string> &cmdTokens);

		/* PING */
		void							handlePing(User &user, std::vector<std::string> &cmdTokens);

		/* PRIVMSG */
		void							privmsgCmd(User &user, std::vector<std::string> &cmdTokens);

		/* QUIT */
		void							quitCmd(User &user, std::vector<std::string> &cmdTokens);

		/* TOPIC */
		void							topicCmd(User &user, std::vector<std::string> &cmdTokens);
		void							clearTopic(std::string channelName);
		void							replyTopic(User &user, std::string channelName);
		void							setTopic(std::string channelName, std::string topic);

		/* USER	 */
		void							userCmd(User &user, std::vector<std::string> &cmdTokens);

		/* reply */
		void							cmdReply(User &user, std::string cmd, std::string firstParam);

		void							errorReply(User &user, std::string reason);

		void							numericReply(User &user, std::string num, std::string msg);
		void							numericReply(User &user, std::string num, std::string firstParam, std::string msg);
		void							numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string msg);
		void							numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string thirdParam, std::string msg);
		void							initNum(void);

		/* send */
		void							sendCmdToUser(User &from, std::string cmd, User &target, std::string msg);
		void							sendCmdToChannel(User &from, std::string cmd, std::set<int> &targets, std::string channel, std::string msg);

		void							sendMsgToChannel(User &sender, std::string &target, std::string &msg);
		void							sendMsgToUser(User &sender, std::string &target, std::string &msg);
		bool							parseTargetPrefix(const std::string &target);
		bool							checkUserPermissions(User &user, Channel &channel);

		/* utils */
		void							addChannel(Channel &channel, std::string &channelName);
		bool							channelExists(std::string channelName);
		int								checkChannelName(std::string channelName);
		void							deleteChannel(const std::string channelName);
		void							removeUserFromChannel(User &user, Channel &channel);

		int								getUserSocket(std::string& nickname);
		std::string						getUserNickname(int socket);
		void							registerUser(User &user);
		bool							isNickAvailable(std::string &nickname);
		void							logoutUser(User &user);


};

# endif