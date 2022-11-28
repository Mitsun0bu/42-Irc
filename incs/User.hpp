/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:23:53 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/28 17:30:26 by llethuil         ###   ########lyon.fr   */
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
		bool					_validPasswd;
		bool					_isAuthenticated;
		std::set<std::string>	_locations;
		std::string				_ip;
		std::string 			_nickname;
		std::string 			_username;
		std::string 			_realname;
		std::string				_cmdReceived;
		int						_cmdToExecute;

		/* member functions												*/
		void					setIp(void);
		void					addLocation(std::string channelName);
		bool					isOperator(std::set<int> operatorSet);

		/* destructor													*/
								~User(void);

	private :

		/* private attributes											*/

		/* private methods												*/
		const void*				getInAddr(void);
};

# endif