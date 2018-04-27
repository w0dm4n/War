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
					srcs/utils/windows_file_get_contents.c						\
					srcs/utils/windows_file_put_contents.c						\
					srcs/utils/file_put_contents.c								\
					srcs/utils/array_length.c									\
					srcs/utils/free_array.c										\
					srcs/utils/file_base_name.c									\
					srcs/utils/file_absolute_path.c								\
					srcs/utils/ft_split_string.c								\
					srcs/utils/ft_dstrjoin.c									\
					srcs/utils/ft_strtoupper.c									\
					srcs/utils/asprintf.c										\
					srcs/binary/get_windows_binary_type.c						\
					srcs/binary/is_windows_binary_file.c						\
					srcs/binary/get_pe_signature.c								\
					srcs/binary/is_pe_signature.c								\
					srcs/binary/is_pe_x64.c										\
					srcs/binary/get_section.c									\
					srcs/binary/OffsetToRVA.c									\
					srcs/binary/RVAToOffset.c									\
					srcs/find_pattern.c											\
					srcs/main.c													\
					srcs/get_files_type.c										\
					srcs/infection.c											\
					srcs/pack_executables.c										\
					srcs/run_packed_executable.c								\

O_FILE = $(SRCS:.c=.o)

FLAGS = -mwindows -ggdb -std=gnu99 -m64

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
