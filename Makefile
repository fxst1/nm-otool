# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/12 10:57:23 by fxst1             #+#    #+#              #
#    Updated: 2018/03/14 12:00:25 by fxst1            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
CC = cc
FLAGS = -Wall -Wextra -Werror -g
INC = -I inc/

OBJDIR = ./obj/

SRC := $(filter %.c, $(shell find src -type f))
OBJ = $(addprefix $(OBJDIR), $(SRC:.c=.o))

NM = nm
OTOOL = otool

all: $(NM) $(OTOOL)

$(NM) : $(OBJ)
	$(CC) $(FLAGS) $(INC) -c main/$(NM).c -o $(OBJDIR)$(NM).o
	$(CC) $(FLAGS) $(INC) -o $(NM) $(OBJ) $(OBJDIR)$(NM).o

$(OTOOL) : $(OBJ)
	$(CC) $(FLAGS) $(INC) -c main/$(OTOOL).c -o $(OBJDIR)$(OTOOL).o
	$(CC) $(FLAGS) $(INC) -o $(OTOOL) $(OBJ) $(OBJDIR)$(OTOOL).o

$(OBJDIR)%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(INC) -o $@ -c $<

clean:
	@rm -rf $(OBJDIR)

fclean:
	@rm -rf $(OBJDIR)
	@rm -rf $(NM)
	@rm -rf $(OTOOL)

re: fclean all

push:
	git add --all
	git commit -am "fix"
	git push
