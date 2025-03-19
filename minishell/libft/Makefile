# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/15 20:18:14 by rcesar-d          #+#    #+#              #
#    Updated: 2025/02/05 10:19:07 by rcesar-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = src/
INC = inc/
OBJ = obj/

CFILES = $(addprefix $(SRC), ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
		ft_isprint.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c ft_strlcat.c \
		ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c \
		ft_strlcpy.c ft_memcmp.c ft_memchr.c ft_calloc.c ft_strdup.c ft_substr.c \
		ft_strjoin.c ft_itoa.c ft_strtrim.c ft_split.c ft_strmapi.c ft_striteri.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
		ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
		ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c \
		ft_printf.c ft_printchar.c ft_printstr.c ft_printnbr.c ft_printnbrhex.c \
		ft_printptr.c ft_strcmp.c get_next_line.c get_next_line_utils.c)

$(OBJ)%.o:	$(SRC)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

OFILES = $(CFILES:$(SRC)%.c=$(OBJ)%.o)

CFLAGS = -Wall -Wextra -Werror -I./inc
CC = @cc $(CFLAGS)

all: $(NAME)

$(NAME): $(OFILES)
	@ar rcs $(NAME) $(OFILES)

clean:
	@rm -rf $(OBJ)
	@rm -f $(OFILES)
	
fclean: clean
	@rm -f $(NAME)

re: clean fclean all

.PHONY: all clean fclean re
