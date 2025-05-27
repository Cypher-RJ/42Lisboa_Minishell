#include "../../includes/minishell.h"

int	is_single_quoted(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	return ((len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		|| (len >= 2 && str[0] == '"' && str[len - 1] == '"'));
}

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

int	is_only_spaces(char *input)
{
	while (*input)
	{
		if (*input != ' ' && *input != '\t')
			return (0);
		input++;
	}
	return (1);
}

int	has_unclosed_quotes(char *input)
{
	int	i;
	int	in_s;
	int	in_d;

	i = 0;
	in_s = 0;
	in_d = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_d && (i == 0 || input[i - 1] != '\\'))
			in_s = !in_s;
		else if (input[i] == '"' && !in_s && (i == 0
				|| input[i - 1] != '\\'))
			in_d = !in_d;
		i++;
	}
	if (in_s)
		return (print_syntax_error("minishell: syntax error: "
				"unclosed single quote\n"));
	if (in_d)
		return (print_syntax_error("minishell: syntax error: "
				"unclosed double quote\n"));
	return (0);
}

static void	update_quote_state(char c, int *in_s, int *in_d, int i, char *input)
{
	if (c == '\'' && !*in_d && (i == 0 || input[i - 1] != '\\'))
		*in_s = !*in_s;
	else if (c == '"' && !*in_s && (i == 0 || input[i - 1] != '\\'))
		*in_d = !*in_d;
}

int	check_syntax(char *input)
{
	int	i;
	int	in_s;
	int	in_d;
	int	expect;

	i = 0;
	in_s = 0;
	in_d = 0;
	expect = 1;
	while (input[i])
	{
		update_quote_state(input[i], &in_s, &in_d, i, input);
		if (!in_s && !in_d)
		{
			if (check_pipe_and_ampersand(input, &i, &expect))
				return (1);
			else
				i++;
			continue ;
		}
		i++;
	}
	if (expect)
		return (print_syntax_error("minishell: syntax error: "
				"unexpected end of input\n"));
	return (has_unclosed_quotes(input));
}

static void	update_redir_quote_state(char c, int *in_s, int *in_d, int i,
		char *input)
{
	if (c == '\'' && !*in_d && (i == 0 || input[i - 1] != '\\'))
		*in_s = !*in_s;
	else if (c == '"' && !*in_s && (i == 0 || input[i - 1] != '\\'))
		*in_d = !*in_d;
}

static int	handle_redirection_syntax(char *input, int *i)
{
	if (input[*i + 1] == '>' || input[*i + 1] == '<')
	{
		if (input[*i + 2] == '>' || input[*i + 2] == '<')
			return (print_syntax_error("minishell: syntax error near "
					"unexpected token `>'\n"));
		(*i)++;
	}
	(*i)++;
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (!input[*i] || input[*i] == '>' || input[*i] == '<'
		|| input[*i] == '|')
		return (print_syntax_error("minishell: syntax error near "
				"unexpected token `newline'\n"));
	return (0);
}

int	check_syntax_redir(char *input)
{
	int	i;
	int	expect_command;
	int	in_s;
	int	in_d;

	i = 0;
	expect_command = 1;
	in_s = 0;
	in_d = 0;
	while (input[i])
	{
		update_redir_quote_state(input[i], &in_s, &in_d, i, input);
		if (!in_s && !in_d)
		{
			if (input[i] == '>' || input[i] == '<')
			{
				if (handle_redirection_syntax(input, &i))
					return (1);
				expect_command = 0;
				continue ;
			}
			else if (input[i] != ' ' && input[i] != '\t')
				expect_command = 0;
		}
		i++;
	}
	if (expect_command)
		return (print_syntax_error("minishell: syntax error near "
				"unexpected token `newline'\n"));
	return (0);
}
