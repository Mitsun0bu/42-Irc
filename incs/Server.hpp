/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:59 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/18 14:38:44 by llethuil         ###   ########lyon.fr   */
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
		t_num							num;
		t_fdList						clientFdList;
		std::string						_date;
		std::map<int, User>				_users;
		std::map<std::string, Channel>	_channels;

		/* member functions												*/
		int								bindSocket(int serverSocket, struct sockaddr_in& socketAddr);
		void							setSocketAddr(struct sockaddr_in& socketAddr, const char* internetHostAddr, int port);
		int								setSocket(void);
		void							clientFdListInit(void);
		void							selectClientSocket(void);
		void							searchForData(void);
		void							initNum(void);



		/* destructor													*/
										~Server(void);

	private :

		/* copy constructor												*/
										Server(const Server& src);

		/* operator overload											*/
		Server							&operator=(const Server& src);

		/* private attributes											*/

		/* private methods												*/
		void							acceptNewUser();

		void							handleClientData(int &currentFd);
		void							printRecvError(int byteCount, int currentFd);

		int								findCmdToExecute(std::string &cmd);
		void							execCmd(User &user, std::vector<std::string> &cmd);

		int								checkChannelName(std::string channelName);

		void							handleQuitCmd(User &user, std::vector<std::string> &cmdTokens);
		void							handlePassCmd(User &user, std::vector<std::string> &cmdTokens);

		void							handleNickCmd(User &user, std::vector<std::string> &cmdTokens);

		void							handleUserCmd(User &user, std::vector<std::string> &cmdTokens);

		void							execPong(User &user, std::vector<std::string> &cmdTokens);

		void							handleJoinCmd(User &user, std::vector<std::string> &cmdTokens);
		int								joinExistingChannel(User &user, std::string channelName, std::string channelKey);
		void							joinNewChannel(User &user, std::string channelName, std::string channelKey);
		void							addChannel(Channel &channel, std::string &name);

		void							handlePartCmd(User &user, std::vector<std::string> &cmdTokens);

		void							handleTopicCmd(User &user, std::vector<std::string> &cmdTokens);
		void							replyTopic(User &user, std::string channelName);
		void							clearTopic(std::string channelName);
		void							setTopic(std::string channelName, std::string topic);

		void							handleNamesCmd(User &user, std::vector<std::string> &cmdTokens);

		void							handleListCmd(User &user, std::vector<std::string> &cmdTokens);

		void							numericReply(User &user, std::string num, std::string msg);
		void							numericReply(User &user, std::string num, std::string firstParam, std::string msg);
		void							numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string msg);
		void							numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string thirdParam, std::string msg);
		void							cmdReply(User &user, std::string cmd, std::string param);

		void							logoutUser(User &user);
		void							sendError(User &user, std::string reason);
		bool							isNickAvailable(std::string &nickname);
		bool							parseNick(std::string &nickname);
		void							registerUser(User &user);
};

# endif