/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:23:53 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/07 17:46:14 by llethuil         ###   ########lyon.fr   */
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
		std::string				_passwd;
		std::string 			_nickname;
		std::string 			_username;
		std::string 			_realname;
		std::set<std::string>	_channels;
		// bool					_pass;
		// unsigned int			_id;
		// std::string			_cmd;
		// std::string			_hostname;
		// std::set<int> 		_privMsg;

		/* member functions												*/
		const void*				getInAddr(void);
		const char*				getIp(void);

		/* destructor													*/
								~User(void);

	// private :

		/* private attributes											*/

};

# endif