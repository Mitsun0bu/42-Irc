/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:23:53 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/02 16:27:37 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# ifndef USER_HPP
# define USER_HPP

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

class User
{
	public :
		/* constructors													*/
								User(void);
								User(
										int						socket,
										struct sockaddr_storage	socketAddr,
										socklen_t				socketAddrSize
									);

		/* copy constructor												*/
								User(const User& src);

		/* operator overload											*/
		User					&operator=(const User& src);

		/* private attributes											*/
		int						_socket;
		struct sockaddr_storage	_socketAddr;
		socklen_t				_socketAddrSize;

		/* member functions												*/
		int						acceptServerConnection(int socket, sockaddr* socketAddr, socklen_t* socketAddrSize);

		/* exceptions													*/
		// class GradeTooHighException : public std::exception
		// {
		// 	public:
		// 		virtual const char *what() const throw();
		// };

		/* destructor													*/
								~User(void);

	// private :

		/* private attributes											*/

};

# endif