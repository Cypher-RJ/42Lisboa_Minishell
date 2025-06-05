#include "../../includes/minishell.h"

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
		return (print_syntax_error("error: unclosed single quote"));
	if (in_d)
		return (print_syntax_error("error: unclosed double quote"));
	return (0);
}

static void	update_quote_state(char c, int *pos, char *input)
{
	if (c == '\'' && !pos[2] && (pos[0] == 0 || input[pos[0] - 1] != '\\'))
		pos[1] = !pos[1];
	else if (c == '"' && !pos[1] && (pos[0] == 0 || input[pos[0] - 1] != '\\'))
		pos[2] = !pos[2];
}

int	check_syntax(char *input)
{
	int	pos[4];

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	pos[3] = 1;
	while (input[pos[0]])
	{
		update_quote_state(input[pos[0]], pos, input);
		if (!pos[1] && !pos[2])
		{
			if (check_pipe_and_ampersand(input, pos))
				return (1);
			else
				pos[0]++;
			continue ;
		}
		pos[0]++;
	}
	if (pos[3])
		return (print_syntax_error("error: unexpected end of input"));
	return (has_unclosed_quotes(input));
}
