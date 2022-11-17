/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirardi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:26:54 by llethuil          #+#    #+#             */
/*   Updated: 2022/11/17 16:36:36 by agirardi         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ DEFINES ~~~                              */
/*                                                                            */
/* ************************************************************************** */

# ifndef MAIN_HPP
# define MAIN_HPP

# define BLUE					"\x1B[34m"
# define CYAN					"\x1B[36m"
# define GREEN					"\x1B[32m"
# define MAGENTA				"\x1B[35m"
# define ORANGE					"\x1B[34m"
# define PURPLE					"\x1B[35m"
# define RED					"\x1B[31m"
# define WHITE					"\x1B[97m"
# define YELLOW					"\x1B[33m"
# define BBLUE					"\x1B[94m"
# define BCYAN					"\x1B[96m"
# define BGREEN					"\x1B[92m"
# define BMAGENTA				"\x1B[95m"
# define BRED					"\x1B[91m"
# define BYELLOW				"\x1B[93m"
# define END					"\033[0m"

# define PASS			0
# define CAP			1
# define NICK			2
# define USER			3
# define PING			4
# define PONG			5
# define QUIT			6
# define JOIN			7
# define PART			8
# define TOPIC		9
# define NAMES		10
# define LIST			11
# define INVITE		12
# define KICK			13
# define MODE			14
# define PRIVMSG	15
# define NOTICE		16

# define FAILED		-1

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ INCLUDES ~~~                             */
/*                                                                            */
/* ************************************************************************** */

# include <algorithm>
# include <csignal>
# include <cstring>
# include <iostream>
# include <map>
# include <set>
# include <sstream>
# include <string>
# include <vector>
#include <ctime>

# include <arpa/inet.h>
# include <fcntl.h>
# include <netdb.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>


/* ************************************************************************** */
/*                                                                            */
/*                              ~~~ TYPEDEFS ~~~                              */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_num
{
	std::string	ERR_ALREADYREGISTERED;
	std::string	ERR_BADCHANMASK;
	std::string	ERR_BADCHANNELKEY;
	std::string	ERR_BANNEDFROMCHAN;
	std::string	ERR_CHANNELISFULL;
	std::string	ERR_ERRONEUSNICKNAME;
	std::string	ERR_INVITEONLYCHAN;
	std::string	ERR_NEEDMOREPARAMS;
	std::string	ERR_NICKNAMEINUSE;
	std::string	ERR_NONICKNAMEGIVEN;
	std::string	ERR_NOSUCHCHANNEL;
	std::string	ERR_NOTONCHANNEL;
	std::string ERR_NOTREGISTERED;
	std::string	ERR_PASSWDMISMATCH;
	std::string	ERR_TOOMANYCHANNELS;
	std::string ERR_UNKNOWNCOMMAND;

	std::string	MSG_ERR_ALREADYREGISTERED;
	std::string	MSG_ERR_BADCHANMASK;
	std::string	MSG_ERR_BADCHANNELKEY;
	std::string	MSG_ERR_BANNEDFROMCHAN;
	std::string	MSG_ERR_CHANNELISFULL;
	std::string	MSG_ERR_ERRONEUSNICKNAME;
	std::string	MSG_ERR_INVITEONLYCHAN;
	std::string	MSG_ERR_NEEDMOREPARAMS;
	std::string	MSG_ERR_NICKNAMEINUSE;
	std::string	MSG_ERR_NONICKNAMEGIVEN;
	std::string	MSG_ERR_NOSUCHCHANNEL;
	std::string	MSG_ERR_NOTONCHANNEL;
	std::string MSG_ERR_NOTREGISTERED;
	std::string	MSG_ERR_PASSWDMISMATCH;
	std::string	MSG_ERR_TOOMANYCHANNELS;
	std::string MSG_ERR_UNKNOWNCOMMAND;

	std::string	RPL_CREATED;
	std::string	RPL_ENDOFNAMES;
	std::string	RPL_MYINFO;
	std::string	RPL_NAMREPLY;
	std::string	RPL_NOTOPIC;
	std::string	RPL_TOPIC;
	std::string	RPL_TOPICWHOTIME;
	std::string	RPL_WELCOME;
	std::string	RPL_YOURHOST;

	std::string	MSG_RPL_CREATED;
	std::string	MSG_RPL_ENDOFNAMES;
	std::string	MSG_RPL_MYINFO;
	std::string	MSG_RPL_NOTOPIC;
	std::string	MSG_RPL_TOPICWHOTIME;
	std::string	MSG_RPL_YOURHOST;
	std::string	MSG_RPL_WELCOME;
}	t_num;

typedef struct s_fdList
{
	// MAXIMUM FD NUMBER
	int				max;
	// MASTER FD LIST
	fd_set			master;
	// TEMPORARY FD LIST FOR SELECT()
	fd_set			read;
	// TEMPORARY FD LIST FOR SELECT()
	fd_set			write;
	// TIME STRUCT TO SPECIFIY A TIMEOUT PERIOD
	struct timeval	t;

}	t_fdList;


# include "Channel.hpp"
# include "Server.hpp"
# include "User.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                              ~~~ PROTOTYPES ~~~                            */
/*                                                                            */
/* ************************************************************************** */

class Server;
class User;

int													main(int argc, char** av);
void												launchServer(Server &s);
int													sendMsg(const int socket, std::string msg);
std::pair<int, std::string>	parseArguments(int ac, char** av);
void												tokenizer(std::string str, const char *del, std::vector<std::string> &tokens);
std::string									intToStr (int n);
std::string									getDate(void);

# endif