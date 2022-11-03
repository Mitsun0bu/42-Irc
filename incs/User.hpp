/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:23:53 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/03 17:38:05 by llethuil         ###   ########lyon.fr   */
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

		/* copy constructor												*/
								User(const User& src);

		/* operator overload											*/
		User					&operator=(const User& src);

		/* private attributes											*/
		int						_socket;
		struct sockaddr_storage	_socketAddr;
		socklen_t				_socketAddrSize;
		char					_remoteIP[INET6_ADDRSTRLEN];
		const void*				_inAddr;
		const char*				_ip;

		/* member functions												*/
		const void*				getInAddr(void);
		const char*				getIp(void);

		/* destructor													*/
								~User(void);

	// private :

		/* private attributes											*/

};

# endif