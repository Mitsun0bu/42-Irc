/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:23:53 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/09 14:57:42 by agirardi         ###   ########lyon.fr   */
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
		// std::string			_hostname;
		bool									_pass;
		bool									_isAuthenticated;
		std::set<std::string>	_channels;
		std::string						_ip;
		std::string 					_nickname;
		std::string 					_username;
		std::string 					_realname;

		/* member functions												*/
		void					setIp(void);

		/* destructor													*/
								~User(void);

	private :

		/* private attributes											*/

		/* private methods												*/
		const void*				getInAddr(void);
};

# endif