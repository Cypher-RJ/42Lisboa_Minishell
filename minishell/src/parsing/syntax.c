#include "../../includes/minishell.h"
/* 
int	is_single_quoted(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	return ((len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
		|| (len >= 2 && str[0] == '"' && str[len - 1] == '"'));
}
 */
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
