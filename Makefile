NAME = pipex
LIBFT = libft_v2/libft.a
LIBFT_DIR = libft_v2

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(LIBFT_DIR)
#-fsanitize=address -fno-omit-frame-pointer 
#-Ibonus

CFILES = src/pipex.c utils/split_for_parse.c utils/utils.c

#BONUS_FILES = bonus/bonus.c bonus/bonus-utils.c \
		gnl/get_next_line_utils.c gnl/get_next_line.c

OBJ = $(CFILES:.c=.o)
#BONUS_OBJ = $(BONUS_FILES:.c=.o)

%.o: %.c
	@echo "compiling..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT)
	@echo "done!"

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)
	
all: $(NAME)
	
clean:
	@rm -f $(OBJ)
	@echo "object files deleted"
#$(BONUS_OBJ)

fclean: clean
	@rm -f $(NAME)
	@make --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "binaries files deleted"

re: fclean all

#bonus: clean
#$(CC) $(CFLAGS) $(OBJ) $(BONUS_FILES) -o $(NAME)

.PHONY: all clean fclean re bonus