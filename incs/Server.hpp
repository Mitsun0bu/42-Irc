/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:59 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/09 15:21:06 by llethuil         ###   ########lyon.fr   */
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
		int								_socket;
		struct sockaddr_in				_socketAddr;
		const char*						_internetHostAddr;
		std::map<int, User>				_users;
		std::map<std::string, Channel>	_channels;
		int								_nCmd;
		int								_cmdToExecute;

		/* member functions												*/
		int					bindSocket(int serverSocket, struct sockaddr_in& socketAddr);
		void				setSocketAddr(struct sockaddr_in& socketAddr, const char* internetHostAddr, int port);
		int					setSocket(void);
		void				selectClientSocket(t_fdList *clientFd);
		void				searchForData(t_fdList *clientFdList);

		/* destructor													*/
							~Server(void);

	private :

		/* copy constructor												*/
							Server(const Server& src);

		/* operator overload											*/
		Server				&operator=(const Server& src);

		/* private attributes											*/

		/* private methods												*/
		void				acceptNewUser(t_fdList *clientFdList);

		void				handleClientData(t_fdList *clientFdList, int* currentFd);
		void				printRecvError(int byteCount, int currentFd);

		void				setCmdToExecute(std::string);
		void				execCmd(User &user, std::string cmd);
		void				execJoin(User &user, std::vector<std::string> &cmdTokens);

		void				numericReply(User &user, int numReply, std::string msg);
		void				numericReply(User &user, int numReply, std::string &cmd, std::string msg);

		void				sendClientData(t_fdList *clientFdList, int* currentFd, char* buffer, int byteCount);
};

# endif