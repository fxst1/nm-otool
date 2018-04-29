# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/12 10:57:23 by fxst1             #+#    #+#              #
#    Updated: 2018/04/28 11:15:05 by fxst1            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
CC = cc
FLAGS = -Wall -Wextra -Werror -g
INC = -I inc/ -I libft/
LIBFT = ./libft.a

OBJDIR = ./obj/

SRC := $(filter %.c, $(shell find src -type f))
OBJ = $(addprefix $(OBJDIR), $(SRC:.c=.o))

NM = ft_nm
OTOOL = ft_otool

all: $(NM) #$(OTOOL)

$(NM) : $(LIBFT) $(OBJ)
	$(CC) $(FLAGS) $(INC) -c main/ft_nm_main.c -o $(OBJDIR)ft_nm_main.o
	$(CC) $(FLAGS) $(INC) -o $(NM) $(OBJ) $(OBJDIR)ft_nm_main.o -L ./libft/ -lft

#$(OTOOL) : $(LIBFT) $(OBJ)
#	$(CC) $(FLAGS) $(INC) -c main/ft_otool_main.c -o $(OBJDIR)ft_otool_main.o
#	$(CC) $(FLAGS) $(INC) -o $(OTOOL) $(OBJ) $(OBJDIR)ft_otool_main.o  -L ./libft/ -lft

$(OBJDIR)%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(INC) -o $@ -c $<

$(LIBFT):
	make -C libft

clean:
	@rm -rf $(OBJDIR)

fclean:
	@rm -rf $(OBJDIR)
	@rm -rf $(NM)
	@rm -rf $(OTOOL)

re: fclean all

push: fclean
	git add --all
	git commit -am "fix"
	git push
