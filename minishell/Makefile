NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = src/builtins_cd_pwd_utils.c \
		src/builtins_cd_utils.c \
		src/builtins_exit_utils.c \
		src/builtins_export_env_utils.c \
		src/builtins_export_utils.c \
		src/builtins_special.c \
		src/builtins_unset_utils.c \
		src/builtins.c \
		src/env.c \
		src/heredoc_expansion.c \
		src/heredoc_functions.c \
		src/execution.c \
		src/free.c \
		src/main.c \
		src/path.c \
		src/pipe_functions.c \
		src/pipes.c \
		src/prompt.c \
		src/redirection.c \
		src/signals_heredoc.c \
		src/signals.c \
		src/utils.c \
\
		src/env_utils.c \
		src/env_utils_dollar.c \
		src/handle_input_utils.c \
		src/parsing/build_redir.c \
		src/parsing/parser_utils_quotes.c \
		src/parsing/parser_split_cmds.c \
		src/parsing/parser_splic_cmds_utils.c \
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