/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <lucas.lethuillier@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 10:23:07 by llethuil          #+#    #+#             */
/*   Updated: 2023/05/08 15:20:12 by llethuil         ###   ########.fr       */
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
		/* constructors											*/
		Channel(void);
		Channel(std::string name);


		/* member functions										*/
		void			addMember(User &user);
		bool			isMember(int userSocket);
		void			addOperator(int userSocket);
		void			addOperator(User &user);
		void			removeOperator(int userSocket);
		void			unsetKey(void);


		/* getters												*/
		std::string		getName(void) const;
		std::string		getKey(void) const;
		bool			getRequiresKey(void) const;
		std::string		getTopic(void) const;
		bool			getTopicIsSet(void) const;
		std::set<int>	&getMembers(void);
		std::set<int>	&getAllowedMembers(void);
		std::set<int>	&getBannedMembers(void);
		std::set<int>	&getOperators(void);
		std::string		getModeKey(void) const;
		std::string		getModeInvite(void) const;

		/* setters												*/
		void			setKey(std::string key);
		void			setName(std::string name);
		void			setRequiresKey(bool isRequired);
		void			setTopic(std::string topic);
		void			setTopicIsSet(bool isTopicSet);
		void			setMembers(std::set<int> members);
		void			setBannedMembers(std::set<int> bannedMembers);
		void			setOperators(std::set<int> operators);
		void			setModeKey();
		void			unsetModeKey();
		void			setModeInvite();
		void			unsetModeInvite();

		/* destructor													*/
							~Channel(void);

	private :
		/* private attributes											*/
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
};

# endif