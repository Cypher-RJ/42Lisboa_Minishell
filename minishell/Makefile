NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = src/main.c \
		src/execution.c \
		src/redirection.c \
		src/pipes.c \
		src/signals.c \
		src/path.c \
		src/env.c \
		src/utils.c \
		src/prompt.c \
		src/parcing/parser.c \
		src/parcing/syntax.c

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