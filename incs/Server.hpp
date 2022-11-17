/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:59 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/17 15:37:57 by agirardi         ###   ########lyon.fr   */
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

class User;
class Channel;

class Server
{
	public :
		/* constructors													*/
							Server(void);
							Server(
									int					port,
									std::string	passwd,
									int					addressFamily,
									int					socketType,
									int					socketFlag,
									int					socketBlockingMode,
									int					protocol,
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
		int					bindSocket(int serverSocket, struct sockaddr_in& socketAddr);
		void				setSocketAddr(struct sockaddr_in& socketAddr, const char* internetHostAddr, int port);
		int					setSocket(void);
		void				clientFdListInit(void);
		void				selectClientSocket(void);
		void				searchForData(void);
		void				initNum(void);



		/* destructor													*/
							~Server(void);

	private :

		/* copy constructor												*/
							Server(const Server& src);

		/* operator overload											*/
		Server				&operator=(const Server& src);

		/* private attributes											*/

		/* private methods												*/
		void				acceptNewUser();

		void				handleClientData(int &currentFd);
		void				printRecvError(int byteCount, int currentFd);

		int					findCmdToExecute(std::string &cmd);
		void				execCmd(User &user, std::vector<std::string> &cmd);
		void				execQuit(User &user, std::vector<std::string> &cmdTokens);
		void				execPass(User &user, std::vector<std::string> &cmdTokens);
		void				execNick(User &user, std::vector<std::string> &cmdTokens);
		void				execUser(User &user, std::vector<std::string> &cmdTokens);
		void				execPong(User &user, std::vector<std::string> &cmdTokens);
		void				execJoin(User &user, std::vector<std::string> &cmdTokens);
		void				addChannel(Channel &channel, std::string name);

		void				execNames(User &user, std::vector<std::string> &cmdTokens);

		void				numericReply(User &user, std::string num, std::string msg);
		void				numericReply(User &user, std::string num, std::string firstParam, std::string msg);
		void				numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string msg);
		void				numericReply(User &user, std::string num, std::string firstParam, std::string secondParam, std::string thirdParam, std::string msg);
		void				cmdReply(User &user, std::string cmd, std::string param);

		void				logoutUser(User &user);
		void				sendError(User &user, std::string reason);
		bool				isNickAvailable(std::string &nickname);
		bool				parseNick(std::string &nickname);
		void				registerUser(User &user);
};

# endif