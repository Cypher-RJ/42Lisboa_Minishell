/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:26:44 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:26:45 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_quotes(int *in_quotes, char *quote_char, int i, char *input)
{
	char	c;

	c = input[i];
	if ((c == '"' || c == '\'') && (i == 0 || input[i - 1] != '\\'))
	{
		if (!*in_quotes)
		{
			*quote_char = c;
			*in_quotes = 1;
		}
		else if (c == *quote_char)
			*in_quotes = 0;
	}
}

int	handle_segment(char *input, char **arg_slot, int *i)
{
	int		start;
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = 0;
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	start = *i;
	while (input[*i])
	{
		handle_quotes(&in_quotes, &quote_char, *i, input);
		if (input[*i] == '|' && !in_quotes)
			break ;
		(*i)++;
	}
	if (start == *i)
		return (1);
	*arg_slot = ft_substr(input, start, *i - start);
	if (!*arg_slot)
		return (0);
	return (*arg_slot != NULL);
}

static int	handle_ampersand(char *input, int i)
{
	if (input[i] == '&' && input[i + 1] == '&')
		return (print_syntax_error("error near unexpected token `&&'"));
	return (0);
}

static int	handle_pipe(char *input, int *pos)
{
	if (input[pos[0]] == '|')
	{
		pos[0]++;
		while (input[pos[0]] == ' ')
			pos[0]++;
		if (!input[pos[0]] || input[pos[0]] == '|')
			return (print_syntax_error("error near unexpected token `|'"));
		pos[3] = 1;
		return (0);
	}
	return (0);
}

int	check_pipe_and_ampersand(char *input, int *pos)
{
	if (handle_ampersand(input, pos[0]))
		return (1);
	if (handle_pipe(input, pos))
		return (1);
	if (input[pos[0]] != ' ' && input[pos[0]] != '\t')
		pos[3] = 0;
	return (0);
}
