NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = src/main.c src/parser.c src/execution.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I includes
LIBFT	=	./libft/libft.a

all: $(NAME)

$(LIBFT):
	@make -s -C ./libft

$(NAME): $(OBJS)
	$(LIBFT) $(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -s clean -C ./libft

fclean: clean
	rm -f $(NAME)
	@make -s fclean -C ./libft

re: fclean all
