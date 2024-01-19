NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address -Ibonus

CFILES = src/pipex.c utils/ft_split.c \
		utils/auxiliar.c utils/utils.c

BONUS_FILES = bonus/bonus.c

OBJ = $(CFILES:.c=.o)
BONUS_OBJ = $(BONUS_FILES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(CFILES) -o $(NAME)

$(BONUS_NAME): $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_FILES) -o $(BONUS_NAME)

all: $(NAME)

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

bonus: clean
	$(CC) $(CFLAGS) $(BONUS_FILES) -o $(NAME)

.PHONY: all clean fclean re bonus