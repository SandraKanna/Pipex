CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
ORIGINAL_SRC_DIR = original_src
MYPRINTF_SRC_DIR = myprintf_src
INCLUDES = -Ioriginal_src -Imyprintf_src -Ignl_src

ORIGINAL_FILES = $(addprefix $(ORIGINAL_SRC_DIR)/, \
		ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
		ft_memmove.c ft_strlcpy.c ft_strlcat.c \
		ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c \
		ft_strncmp.c ft_strcmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c  \
		ft_atoi.c ft_calloc.c ft_strdup.c \
		ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c\
		ft_itoa.c ft_striteri.c ft_strmapi.c\
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_lstnew.c ft_lstadd_front.c ft_lstsize.c \
		ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
		ft_lstclear.c ft_lstiter.c ft_lstmap.c)

MYPRINTF_FILES = $(addprefix $(MYPRINTF_SRC_DIR)/, \
ft_printf.c ft_putnbr_base.c ft_put_alpha.c ft_print_p.c)

GNL_FILES = gnl_src/get_next_line.c gnl_src/get_next_line_utils.c

OBJ = $(ORIGINAL_FILES:.c=.o) $(MYPRINTF_FILES:.c=.o) $(GNL_FILES:.c=.o)
NAME = libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@ar -rcs $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
