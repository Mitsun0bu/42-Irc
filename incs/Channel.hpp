/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:23:07 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/09 11:38:29 by llethuil         ###   ########lyon.fr   */
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

		/* public attributes											*/
		std::string			_name;
		std::string			_key;
		bool				_hasKey;
		std::string			_topic;
		bool				_topicIsSet;
		std::set<int>		_members;
		std::set<int>		_operators;
		std::string			_mode;

		/* member functions												*/
		void	addUser(User &user);
		void	addUser(int userSocket);
		void	deleteUser(User &user);
		void	deleteUser(int userSocket);
		void	addOperator(User &user);
		void	addOperator(int userSocket);
		void	deleteOperator(User &user);
		void	deleteOperator(int userSocket);
		// void	sendMsgToMembers(const std::string &content, int socketException) const;

		/* exceptions													*/

		/* destructor													*/
							~Channel(void);

	private :

		/* copy constructor												*/
							Channel(const Channel& src);

		/* operator overload											*/
		Channel				&operator=(const Channel& src);

		/* private attributes											*/

};

# endif