NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = src/builtins_special.c \
		src/builtins.c \
		src/env.c \
		src/execution.c \
		src/free.c \
		src/main.c \
		src/path.c \
		src/pipe_functions.c \
		src/pipes.c \
		src/prompt.c \
		src/redirection.c \
		src/signals.c \
		src/utils.c \
\
		src/parsing/parser.c \
		src/parsing/syntax.c \
		src/parsing/build.c

OBJS = $(SRCS:.c=.o)
INCLUDES = -I includes -I libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT): 
	@ make -s -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	@ $(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME) -lreadline

%.o: %.c
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@ make -s -C $(LIBFT_DIR) clean
	@ rm -f $(OBJS)

fclean: clean
	@ make -s -C $(LIBFT_DIR) fclean
	@ rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re