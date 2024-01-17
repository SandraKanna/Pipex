NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3

CFILES = src/pipex.c utils/ft_split.c \
		utils/auxiliar.c utils/utils.c

OBJ = $(CFILES:.c=.o)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

$(NAME): $(OBJ)
	gcc $(CFLAGS) $(CFILES) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re