#include "../../includes/minishell.h"

int	handle_segment(char *input, char **arg_slot, int *i)
{
	int	start;
	int	in_quotes;
	
	in_quotes = 0;
	while (input[*i] == ' ' && input[*i] == '\t')
		(*i)++;
	start = *i;
	while (input[*i])
	{
		if (input[*i] == '"' && (*i == 0 || input[*i - 1] != '\\'))
			in_quotes = !in_quotes;
		else if (input[*i] == '|' && !in_quotes)
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

int	check_pipe_and_ampersand(char *input, int *i, int *expect)
{
	if (input[*i] == '&' && input[*i + 1] == '&')
		return (print_syntax_error("minishell: syntax error near unexpected token `&&'\n"));
		if (input[*i] == '|')
	{
		(*i)++;
		while (input[*i] == ' ')
		(*i)++;
		if (!input[*i] || input[*i] == '|')
			return (print_syntax_error("minishell: syntax error near unexpected token `|'\n"));
		*expect = 1;
		return (0);
	}
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
		if (input[i] == '\'' && !in_d)
			in_s = !in_s;
			else if (input[i] == '"' && !in_s)
			in_d = !in_d;
			i++;
		}
	if (in_s)
		return (print_syntax_error("minishell: syntax error: unclosed single quote\n"));
	if (in_d)
		return (print_syntax_error("minishell: syntax error: unclosed double quote\n"));
	return (0);
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
		if (input[i] == '\'' && !in_d)
			in_s = !in_s;
		else if (input[i] == '"' && !in_s)
			in_d = !in_d;
		else if (!in_s && !in_d)
			if (check_pipe_and_ampersand(input, &i, &expect))
				return (1);
		i++;
	}
	if (expect)
		return (print_syntax_error("minishell: syntax error: unexpected end of input\n"));
	return (has_unclosed_quotes(input));
}

int check_syntax_redir(char *input)
{
	int i;
	int expect_command;
	
	i = 0;
	expect_command = 1;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<')
		{
		if (input[i + 1] == '>' || input[i + 1] == '<')
            {
				if (input[i + 2] == '>' || input[i + 2] == '<')
					return print_syntax_error("minishell: syntax error near unexpected token `>'\n");
				i++;
			}
			expect_command = 1;
		}
		else if (input[i] != ' ' && input[i] != '\t')
			expect_command = 0;
		i++;
	}
	if (expect_command)
		return print_syntax_error("minishell: syntax error near unexpected token `newline'\n");
	return 0;
}
