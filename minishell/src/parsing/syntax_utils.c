#include "../../includes/minishell.h"

static void	handle_quotes(char c, int *in_quotes, char *quote_char, int i,
		char *input)
{
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
		handle_quotes(input[*i], &in_quotes, &quote_char, *i, input);
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
		return (print_syntax_error("minishell: syntax error near "
				"unexpected token `&&'\n"));
	return (0);
}

static int	handle_pipe(char *input, int *i, int *expect)
{
	if (input[*i] == '|')
	{
		(*i)++;
		while (input[*i] == ' ')
			(*i)++;
		if (!input[*i] || input[*i] == '|')
			return (print_syntax_error("minishell: syntax error near "
					"unexpected token `|'\n"));
		*expect = 1;
		return (0);
	}
	return (0);
}

int	check_pipe_and_ampersand(char *input, int *i, int *expect)
{
	if (handle_ampersand(input, *i))
		return (1);
	if (handle_pipe(input, i, expect))
		return (1);
	if (input[*i] != ' ' && input[*i] != '\t')
		*expect = 0;
	return (0);
}
