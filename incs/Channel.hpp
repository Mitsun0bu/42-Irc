/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:23:07 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/30 15:11:23 by llethuil         ###   ########lyon.fr   */
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

		/* public attributes											*/
		std::string		_name;
		std::string		_key;
		bool			_requiresKey;
		std::string		_topic;
		bool			_topicIsSet;
		std::set<int>	_members;
		std::set<int>	_allowedMembers;
		std::set<int>	_bannedMembers;
		std::set<int>	_operators;
		std::string		_modeKey;
		std::string		_modeInvite;


		/* operator overload											*/
		Channel			&operator=(const Channel& src);

		/* member functions												*/
		void			addMember(User &user);
		bool			isMember(int userSocket);
		void			addOperator(int userSocket);
		void			addOperator(User &user);
		void			removeOperator(int userSocket);
		// void			removeOperator(User &user);
		void			setKey(std::string key);
		void			unsetKey(void);

		// void				addMember(int userSocket);
		// void				deleteMember(User &user);
		// void				deleteMember(int userSocket);

		/* exceptions													*/

		/* destructor													*/
							~Channel(void);

	private :



		/* private attributes											*/

};

# endif