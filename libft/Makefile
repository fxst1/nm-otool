# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/08/11 17:14:40 by fjacquem          #+#    #+#              #
#    Updated: 2019/02/05 20:48:41 by fjacquem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

FLAGS = -Wall -Wextra -Werror
INC = -I ./ -I ../
CC = gcc
HEAD_CPY = /usr/include/libft
LIB_CPY = /usr/lib

ifeq ($(ARCH),)
	FLAGS += -arch x86_64
else
	FLAGS += -arch $(ARCH)
	NAME = libft_$(ARCH).a
endif

SRC_ESS = ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_memchr.c\
	ft_memdel.c ft_memcmp.c ft_strlen.c ft_strdup.c ft_strcpy.c ft_strncpy.c\
	ft_strcat.c ft_strncat.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strstr.c\
	ft_strnstr.c ft_strcmp.c ft_strncmp.c ft_atoi.c ft_isalpha.c ft_isdigit.c\
	ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c\
	ft_memalloc.c ft_strnew.c ft_strdel.c ft_strclr.c\
	ft_striter.c ft_striteri.c ft_strmap.c ft_strmapi.c ft_strequ.c\
	ft_strnequ.c ft_strsub.c ft_strjoin.c ft_strtrim.c ft_strsplit.c\
	ft_itoa.c ft_putchar.c ft_putstr.c ft_putendl.c ft_putnbr.c\
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
	ft_lstnew.c ft_lstdelone.c ft_lstdel.c ft_lstadd.c ft_lstiter.c\
	ft_lstmap.c ft_memccpy.c
HEAD_ESS = libft.h

SRC_STACK = ft_stknew.c ft_stkinit.c ft_stkempty.c ft_stkfull.c ft_stkclear.c\
	ft_stkpop.c ft_stkpush.c
HEAD_STACK = stack.h

SRC_BTREE = ft_btreenew.c ft_btreeadd.c ft_btreefree.c ft_btreeforeach.c
HEAD_BTREE = btree.h

SRC_FCT = ft_lstnew_noalloc.c\
	ft_lstfree.c ft_lstsize.c ft_lstadd_begin.c\
	ft_isspace.c ft_memndel.c\
	ft_strsort.c ft_strcut.c \
	ft_putnstr.c\
	ft_putnbr_base_fd.c ft_putnbr_base.c ft_itoa_base.c ft_print_memory.c\
	ft_strrev.c ft_puthex.c\
	ft_lstadd_end.c ft_memrcpy.c
HEAD_FCT = implemt.h

SRC_GNX =
HEAD_GNX = gnx.h get_next_line.h

SRC_MAP =
HEAD_MAP = map.h

SRC_GARBAGE =
HEAD_ESS = gc.h

HEAD_REST = os.h master.h
HEADS = $(HEAD_ESS) $(HEAD_REST) $(HEAD_MAP) $(HEAD_GNX) $(HEAD_FCT) $(HEAD_BTREE) $(HEAD_STACK)

SRC_ALL = $(SRC_ESS) $(SRC_FCT) $(SRC_MAP) $(BONUS_STACK) $(BONUS_BTREE) $(BONUS_GARBAGE)\
			$(SRC_GNX)

OBJS =	$(addprefix ./obj/,$(SRC_ALL:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
		@ar rc $(NAME) $(OBJS)
		@echo "\033[38;5;27mLibft created\033[0m"
		@ranlib $(NAME)

./obj/%.o: %.c
		@mkdir -p $(@D)
		$(CC) $(FLAGS) $(INC) -c $< -o $@

clean:
		@rm -rf ./obj

fclean: clean
		@rm -f $(NAME)

re:		fclean all

dymlib: $(NAME)
		@echo "\033[38;5;1mMay sure to have root permissions\033[0m"
		@sudo cp -f $(NAME) $(LIB_CPY) 2> /dev/null

dymheader:
		@echo "\033[38;5;1mMay sure to have root permissions\033[0m"
		@chmod 644 $(HEADS)
		@sudo mkdir $(HEAD_CPY) 2> /dev/null || echo " " > /dev/null
		@sudo cp -f $(HEADS) $(HEAD_CPY)

.PHONY: re fclean clean all
