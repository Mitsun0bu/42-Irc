# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llethuil <llethuil@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/31 14:42:54 by llethuil          #+#    #+#              #
#    Updated: 2022/11/04 14:00:20 by llethuil         ###   ########lyon.fr    #
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

SRCS_SUBDIRS	:=	class						\
					main						\
					parsing						\
					server						\

SRCS_LST		:=	class/User.cpp				\
					class/Server.cpp			\
					main/main.cpp				\
					main/clientFdListInit.cpp	\
					main/launchServer.cpp		\
					parsing/parseArguments.cpp	\

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