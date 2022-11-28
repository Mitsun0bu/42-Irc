# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/31 14:42:54 by llethuil          #+#    #+#              #
#    Updated: 2022/11/28 19:28:47 by llethuil         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                              ~~~ COLOR CODES ~~~                             #
#                                                                              #
# **************************************************************************** #

GREEN	:=	\033[32m
BLUE	:=	\033[34m
PINK	:=	\033[35m
END		:=	\033[0m

# **************************************************************************** #
#                                                                              #
#                              ~~~ VARIABLES ~~~                               #
#                                                                              #
# **************************************************************************** #

NAME			:=	ircserv

CC				:=	c++

CXXFLAGS		:=	-std=c++98 -Wall -Wextra -Werror

RM				:=	rm -f

INCS_DIR		:=	incs

INCS_LST		:=	main.hpp					\

SRCS_DIR		:=	srcs

SRCS_SUBDIRS	:=	channel						\
					main						\
					message						\
					parsing						\
					server/commands				\
					server/reply				\
					server/send					\
					server/utils				\
					user						\
					utils						\

SRCS_LST		:=	channel/Channel.cpp				\
					user/User.cpp					\
					server/Server.cpp				\
					server/commands/handleCmd.cpp	\
					server/commands/inviteCmd.cpp	\
					server/commands/joinCmd.cpp		\
					server/commands/kickCmd.cpp		\
					server/commands/listCmd.cpp		\
					server/commands/modeCmd.cpp		\
					server/commands/namesCmd.cpp	\
					server/commands/nickCmd.cpp		\
					server/commands/partCmd.cpp		\
					server/commands/passCmd.cpp		\
					server/commands/pingCmd.cpp		\
					server/commands/privmsgCmd.cpp	\
					server/commands/quitCmd.cpp		\
					server/commands/topicCmd.cpp	\
					server/commands/userCmd.cpp		\
					server/reply/cmdReply.cpp		\
					server/reply/errorReply.cpp		\
					server/reply/numericReply.cpp	\
					server/send/sendCmd.cpp			\
					server/send/sendMsg.cpp			\
					server/utils/utilsChannels.cpp	\
					server/utils/utilsUsers.cpp		\
					main/main.cpp					\
					main/launchServer.cpp			\
					message/sendMsg.cpp				\
					parsing/parseArguments.cpp		\
					parsing/tokenizer.cpp			\
					utils/intToStr.cpp				\

OBJS_DIR		:= .objs

OBJS_LST		:=	$(SRCS_LST:.cpp=.o)

INCS			:=	$(addprefix $(INCS_DIR)/, $(INCS_LST))

SRCS			:=	$(addprefix $(SRCS_DIR)/, $(SRCS_LST))

OBJS			:=	$(addprefix $(OBJS_DIR)/, $(OBJS_LST))

# ************************************************************************** #
#                                                                            #
#                              ~~~ RULES & COMMANDS ~~~                      #
#                                                                            #
# ************************************************************************** #

.SILENT:

all: $(NAME)

$(NAME): $(OBJS)
	test -z '$(filter %.o,$?)' || printf "$(GREEN)> All the .cpp files from $(NAME) have been compiled successfully !$(END)\n"
	printf "$(BLUE)> Creating the executable file :$(END) $@\n"
	$(CC) $(OBJS) -o $(NAME)
	printf "$(GREEN)> Executable file has been created successfully !$(END)\n"

$(OBJS_DIR):
	mkdir -p $(addprefix $(OBJS_DIR)/, $(SRCS_SUBDIRS))

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(INCS) Makefile | $(OBJS_DIR)
	printf "$(BLUE)> Compiling :$(END) $<\n"
	$(CC) $(CXXFLAGS) -I $(INCS_DIR) -c $< -o $@

clean:
	printf "$(PINK)> Removing $(NAME) .o files ...$(END)\n"
	$(RM) $(OBJS)
	printf "$(PINK)> Removing .objs directory ...$(END)\n"
	rm -rf $(OBJS_DIR)
	printf "$(GREEN)> All the .o files have been removed successfully !$(END)\n"

fclean: clean
	printf "$(PINK)> Removing $(NAME) executable file ...$(END)\n"
	$(RM) $(NAME)
	printf "$(GREEN)> $(NAME) executable file have been removed successfully !$(END)\n"

re: fclean all

.PHONY: all clean fclean re