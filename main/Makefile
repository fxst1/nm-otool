NAME=computer
CC=g++
FLAGS=-Wall -Wextra -g
INCLUDE=-I ./inc/ -I ./libft/
MACRO=

SRC := $(filter %.cpp, $(shell find src -type f))
OBJ = $(SRC:.cpp=.o)

LIB = 
LINK = -lcurses -lm

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(FLAGS) $(MACRO) $(INCLUDE) $(OBJ) $(LIB) -o $(NAME)  $(LINK)

%.o: %.cpp
		$(CC) $(FLAGS) $(MACRO) $(INCLUDE) -o $@ -c $< 

clean:
		@rm -vf $(OBJ)
		#make -C $(LIBFT_PATH) clean

fclean: clean
		@rm -vf $(NAME)
		#make -C $(LIBFT_PATH) fclean

install:
	
re: fclean all
