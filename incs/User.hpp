/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:23:53 by llethuil          #+#    #+#             */
/*   Updated: 2022/12/07 17:05:06 by llethuil         ###   ########lyon.fr   */
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

		/* member functions												*/
		void					addLocation(std::string channelName);
		bool					isOperator(std::set<int> operatorSet);

		/* getters												*/
		struct sockaddr_storage	getSocketAddr(void) const;
		socklen_t				getSocketAddrSize(void) const;
		int						getSocket(void) const;
		bool					getValidPasswd(void) const;
		bool					getIsAuthenticated(void) const;
		std::set<std::string>&	getLocations(void);
		std::string				getIp(void) const;
		std::string& 			getNickname(void);
		std::string& 			getUsername(void);
		std::string&			getCmdReceived(void);
		int						getCmdToExecute(void) const;

		/* setters												*/
		void					setSocketAddr(struct sockaddr_storage socketAddr);
		void					setSocketAddrSize(socklen_t socketAddrSize);
		void					setSocket(int socket);
		void					setValidPasswd(bool isValid);
		void					setIsAuthenticated(bool IsAuthenticated);
		void					setLocations(std::set<std::string> locations);
		void 					setNickname(std::string nickname);
		void 					setUsername(std::string username);
		void					setCmdReceived(std::string cmd);
		void					setCmdToExecute(int cmd);

		/* destructor													*/
								~User(void);

	private :

		/* private attributes											*/
		int						_socket;
		struct sockaddr_storage	_socketAddr;
		socklen_t				_socketAddrSize;
		char					_remoteIP[INET6_ADDRSTRLEN];
		const void*				_inAddr;
		bool					_validPasswd;
		bool					_isAuthenticated;
		std::set<std::string>	_locations;
		std::string 			_nickname;
		std::string 			_username;
		std::string				_cmdReceived;
		int						_cmdToExecute;
};

# endif