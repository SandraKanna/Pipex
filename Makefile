NAME = pipex
BONUS = pipex_bonus
LIBFT = libft_v2/libft.a
LIBFT_DIR = libft_v2

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(LIBFT_DIR) -IIncludes

CFILES = src/pipex.c src/utils/split_for_parse.c src/utils/utils.c

BONUS_FILES = bonus/bonus.c bonus/bonus_utils.c

MAIN = src/main.c
BONUS_MAIN = bonus/bonus_main.c

OBJ = $(CFILES:.c=.o)
BONUS_OBJ = $(BONUS_FILES:.c=.o)

%.o: %.c
	@echo "compiling..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT) $(MAIN:.c=.o)
	@$(CC) $(CFLAGS) $(OBJ) $(MAIN:.c=.o) -o $(NAME) $(LIBFT)
	@echo "---You can execute the mandatory part---"

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

$(BONUS): $(BONUS_OBJ) $(OBJ) $(LIBFT) $(BONUS_MAIN:.c=.o)
	@$(CC) $(CFLAGS) $(BONUS_OBJ) $(OBJ) $(BONUS_MAIN:.c=.o) -o $(BONUS) $(LIBFT)
	@echo "---You can execute the bonus part---"

clean:
	@rm -f $(OBJ) $(BONUS_OBJ) src/main.o bonus/bonus_main.o
	@echo "object files deleted"

all: $(NAME)

bonus: $(BONUS)

fclean: clean
	@rm -f $(NAME) $(BONUS)
	@make --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "binaries files deleted"

re: fclean all

.PHONY: all clean fclean re bonus