NAME = pipex
BONUS = pipex-bonus
LIBFT = libft_v2/libft.a
LIBFT_DIR = libft_v2

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(LIBFT_DIR) -Ibonus
#-fsanitize=address -fno-omit-frame-pointer 
#creer dossier Includes avec tous les .h

CFILES = src/pipex.c utils/split_for_parse.c utils/utils.c

BONUS_FILES = bonus/bonus.c bonus/bonus_utils.c

OBJ = $(CFILES:.c=.o)
BONUS_OBJ = $(BONUS_FILES:.c=.o)

%.o: %.c
	@echo "compiling..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT)
	@echo "done!"

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

$(BONUS): $(NAME) $(BONUS_OBJ)
	@$(CC) $(CFLAGS) $(BONUS_OBJ) -o $(BONUS)
	@echo "bonus part"

all: $(NAME)

bonus: $(BONUS)

clean:
	@rm -f $(OBJ) $(BONUS_OBJ)
	@echo "object files deleted"

fclean: clean
	@rm -f $(NAME)
	@make --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "binaries files deleted"

re: fclean all

#bonus: clean
#$(CC) $(CFLAGS) $(OBJ) $(BONUS_FILES) -o $(NAME)

.PHONY: all clean fclean re bonus