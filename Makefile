# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/12 10:57:23 by fxst1             #+#    #+#              #
#    Updated: 2019/02/02 18:05:36 by fjacquem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
CC = cc
FLAGS = -Wall -Wextra -Werror -g
INC = -I inc/ -I libft/
LIBFT = ./libft/libft.a
OBJDIR = ./obj/

SRC := $(filter %.c, $(shell find src -type f))
SRC_NM = $(SRC) main/ft_nm_main.c
SRC_OTOOL = $(SRC) main/ft_otool_main.c

OBJ_NM = $(addprefix $(OBJDIR), $(SRC_NM:.c=.o))
OBJ_OTOOL = $(addprefix $(OBJDIR), $(SRC_OTOOL:.c=.o))

ifeq ($(ARCH),)
	FLAGS += -arch x86_64
	NM = ft_nm
	OTOOL = ft_otool
else
	FLAGS += -arch $(ARCH)
	NM = ft_nm_$(ARCH)
	OTOOL = ft_otool_$(ARCH)
	LIBFT = ./libft/libft_$(ARCH).a
endif

all: $(NM) $(OTOOL)

$(NM) : $(LIBFT) $(OBJ_NM)
	$(CC) $(FLAGS) $(INC) -o $(NM) $(OBJ_NM) -L ./libft/ -lft

$(OTOOL) : $(LIBFT) $(OBJ_OTOOL)
	$(CC) $(FLAGS) $(INC) -o $(OTOOL) $(OBJ_OTOOL) -L ./libft/ -lft

$(OBJDIR)%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(INC) -o $@ -c $<

$(LIBFT):
	make -C libft ARCH=$(ARCH)

all-arch:
	make clean ARCH=i386
	make all ARCH=i386

clean:
	@rm -rf $(OBJDIR)

cleanlib:
	make -C libft fclean

fclean: cleanlib
	@rm -rf $(OBJDIR)
	@rm -rf $(NM)
	@rm -rf $(OTOOL)

re: fclean all
