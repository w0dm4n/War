# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frmarinh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/03/07 12:48:21 by frmarinh          #+#    #+#              #
#    Updated: 2017/03/07 15:00:34 by frmarinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = War

SRCS =	\
		srcs/main.c					\

O_FILE = $(SRCS:.c=.o)

FLAGS = -mwindows -ggdb

#FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	@gcc $(SRCS) $(FLAGS) -I includes/ -o $(NAME) 

clean:
	@del $(NAME).exe

fclean: clean

re:		fclean all
