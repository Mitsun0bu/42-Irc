/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:59 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/03 10:15:37 by llethuil         ###   ########lyon.fr   */
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
		int					_port;
		int					_addressFamily;
		int					_socketType;
		int					_socketFlag;
		int					_socketBlockingMode;
		int					_protocol;
		int					_socket;
		struct sockaddr_in	_socketAddr;
		const char*			_internetHostAddr;

		/* member functions												*/
		int					bindSocket(int serverSocket, struct sockaddr_in& socketAddr);
		void				setSocketAddr(struct sockaddr_in& socketAddr, const char* internetHostAddr, int port);
		int					setSocket();

		/* exceptions													*/
		// class GradeTooHighException : public std::exception
		// {
		// 	public:
		// 		virtual const char *what() const throw();
		// };

		/* destructor													*/
							~Server(void);

	private :

		/* copy constructor												*/
							Server(const Server& src);

		/* operator overload											*/
		Server				&operator=(const Server& src);

		/* private attributes											*/

};

# endif