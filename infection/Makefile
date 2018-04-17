# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/20 18:45:49 by jguyet            #+#    #+#              #
#    Updated: 2018/03/20 18:45:51 by jguyet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

STUDENT			=	jguyet

PROG1			=	infection.exe

WHOAMI_CMD		=	$(shell whoami)

NAMEBASE    	=   $(shell basename $(PROG1) .a)

LENGTHNAME		=	`printf "%s" $(NAMEBASE) | wc -c`

CC				=	gcc

FLAGS			=	-mwindows -ggdb -lcrypto -std=gnu99

SRCDIR			=

OBJDIR			=	.objs/

INCDIR			=	includes/

SRCSPROG1		=	srcs/utils/ft_strnew.c										\
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

DSRCSPROG1		=	$(addprefix $(SRCDIR), $(SRCSPROG1))

OBJSPROG1		=	$(addprefix $(OBJDIR), $(DSRCSPROG1:.c=.o))

.SILENT:

all:
	if test -f $(PROG1) ; then																\
		echo "make: Nothing to be done for \`all\`.";										\
	else																					\
		$(MAKE) -j $(STUDENT);																\
	fi																						\

$(STUDENT):
	$(MAKE) $(PROG1)

$(PROG1):	$(OBJDIR) $(OBJSPROG1)
	$(CC) $(OBJSPROG1) $(FLAGS) -o $(PROG1)
	echo "MAKE    [$(PROG1)]"

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(dir $(OBJSPROG1))

$(OBJDIR)%.o : $(SRCDIR)%.c | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@ -I $(INCDIR)
	printf "\033[38;5;092m%s\033[0m\033[K\n"	"[$(@)]"

clean:
	echo -en "\r\033[38;5;101mCLEAN  "														\
		"[\033[0m$(NAMEBASE)\033[38;5;101m]\033[K\033[0m\n";									\
	rm -rf $(OBJDIR)

fclean:
	echo -en "\r\033[38;5;124mFCLEAN "														\
		"[\033[0m$(NAMEBASE)\033[38;5;124m]\033[K\033[0m\n";									\
	rm -rf $(PROG1)
	rm -rf $(OBJDIR)

re:			fclean all

.PHONY: all clean fclean re
