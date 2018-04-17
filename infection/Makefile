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

NAME = infection

SRCS			=	srcs/utils/ft_strnew.c										\
					srcs/utils/file_exists.c									\
					srcs/utils/is_directory.c									\
					srcs/utils/file_get_contents.c								\
					srcs/utils/file_put_contents.c								\
					srcs/utils/array_length.c									\
					srcs/utils/free_array.c										\
					srcs/utils/file_base_name.c									\
					srcs/utils/file_absolute_path.c								\
					srcs/utils/ft_split_string.c								\
					srcs/utils/ft_dstrjoin.c									\
					srcs/utils/ft_strtoupper.c									\
					srcs/main.c													\
					srcs/get_files_type.c										\
					srcs/infection.c											\
					srcs/aes.c													\
					srcs/binary/get_windows_binary_type.c						\
					srcs/binary/is_windows_binary_file.c						\

O_FILE = $(SRCS:.c=.o)

FLAGS = -mwindows -ggdb -lcrypto -std=gnu99

#FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	@gcc $(SRCS) $(FLAGS) -I includes/ -o $(NAME)

clean:
	@del $(NAME).exe

fclean: clean

re:		fclean all
