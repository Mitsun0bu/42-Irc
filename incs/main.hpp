/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:26:54 by llethuil          #+#    #+#             */
/*   Updated: 2022/10/31 14:58:23 by llethuil         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                               ~~~ DEFINES ~~~                              */
/*                                                                            */
/* ************************************************************************** */

# ifndef MAIN_HPP
# define MAIN_HPP

# define BLUE		"\x1B[34m"
# define CYAN		"\x1B[36m"
# define GREEN		"\x1B[32m"
# define MAGENTA	"\x1B[35m"
# define RED		"\x1B[31m"
# define WHITE		"\x1B[97m"
# define YELLOW		"\x1B[33m"
# define BBLUE		"\x1B[94m"
# define BCYAN		"\x1B[96m"
# define BGREEN		"\x1B[92m"
# define BMAGENTA	"\x1B[95m"
# define BRED		"\x1B[91m"
# define BYELLOW	"\x1B[93m"
# define END		"\033[0m"

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
# include <unistd.h>
# include <vector>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/time.h>


/* ************************************************************************** */
/*                                                                            */
/*                              ~~~ PROTOTYPES ~~~                            */
/*                                                                            */
/* ************************************************************************** */

int							main(int argc, char** av);
std::pair<int, std::string>	parseArguments(int ac, char** av);
int							bindServerSocket(int serverSocket, struct sockaddr_in& serverSocketAddr);
int							setServerSocket(int port);
void						setServerSocketAddr(struct sockaddr_in& serverSocketAddr, const char* internetHostAddress, int port);


# endif