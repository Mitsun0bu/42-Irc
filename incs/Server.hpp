/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:59 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/09 17:05:09 by agirardi         ###   ########lyon.fr   */
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
									int			port,
									int			addressFamily,
									int			socketType,
									int			socketFlag,
									int			socketBlockingMode,
									int			protocol,
									const char*	internetHostAddr
								  );

		/* public attributes											*/
		int								_port;
		int								_addressFamily;
		int								_socketType;
		int								_socketFlag;
		int								_socketBlockingMode;
		int								_protocol;
		const char*						_internetHostAddr;
		int								_socket;
		struct sockaddr_in				_socketAddr;
		t_fdList						clientFdList;
		std::map<int, User>				_users;
		std::map<std::string, Channel>	_channels;
		int								_nCmd;
		int								_cmdToExecute;

		/* member functions												*/
		int					bindSocket(int serverSocket, struct sockaddr_in& socketAddr);
		void				setSocketAddr(struct sockaddr_in& socketAddr, const char* internetHostAddr, int port);
		int					setSocket(void);
		void				clientFdListInit(void);
		void				selectClientSocket(void);
		void				searchForData(void);

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

		void				handleClientData(int* currentFd);
		void				printRecvError(int byteCount, int currentFd);

		void				setCmdToExecute(std::string);
		void				execCmd(User &user, std::string cmd);
		void				execPass(User &user, std::vector<std::string> &cmdTokens);
		void				execJoin(User &user, std::vector<std::string> &cmdTokens);

		void				numericReply(User &user, int numReply, std::string msg);
		void				numericReply(User &user, int numReply, std::string &cmd, std::string msg);

		void				sendClientData(int* currentFd, char* buffer, int byteCount);	
		void				sendError(User &user, std::string reason);
};

# endif