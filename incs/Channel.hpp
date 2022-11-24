/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:23:07 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/24 15:38:53 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
# define CHANNEL_HPP

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

class Channel
{
	public :
		/* constructors													*/
		Channel(void);
		Channel(std::string name);
		/* copy constructor												*/
		Channel(const Channel& src);
		/* public attributes											*/
		std::string		_name;
		std::string		_key;
		bool			_requiresKey;
		std::string		_topic;
		bool			_topicIsSet;
		std::set<int>	_members;
		std::set<int>	_bannedMembers;
		std::set<int>	_operators;
		std::string		_mode;


		/* operator overload											*/
		Channel			&operator=(const Channel& src);

		/* member functions												*/
		void			addMember(User &user);
		void			addOperator(User &user);
		void			addOperator(int socket);
		void			removeOperator(User &user);
		void			setKey(std::string key);
		void			unsetKey(void);

		// void				addMember(int userSocket);
		// void				deleteMember(User &user);
		// void				deleteMember(int userSocket);
		// void				deleteOperator(User &user);
		// void	sendMsgToMembers(const std::string &content, int socketException) const;

		/* exceptions													*/

		/* destructor													*/
							~Channel(void);

	private :



		/* private attributes											*/

};

# endif