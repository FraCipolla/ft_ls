NAME=ls
SRC=main.c dir_list.c
SRCDIR=src
OBJ=$(SRC:%.c=$(SRCDIR)/%.o)
CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
