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
		srcs/aes.c					\

O_FILE = $(SRCS:.c=.o)

FLAGS = -mwindows -ggdb

INCLUDES = -I includes/ -I C:/OpenSSL-Win64/include -I C:/mingw/include

LIBRARY = -L C:/OpenSSL-Win64/lib -lssl -lcrypto

#FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	@gcc $(SRCS) $(INCLUDES) -o $(NAME) $(LIBRARY) $(FLAGS)

clean:
	@del $(NAME).exe

fclean: clean

re:		fclean all
