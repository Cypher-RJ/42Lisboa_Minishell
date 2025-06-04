#include "../../includes/minishell.h"

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
