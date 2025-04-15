#include "../../includes/minishell.h"

static int	count_words(char *str)
{
	int	count;
	int	in_word;
	int	in_quotes;
	int	i;

	count = 0;
	in_word = 0;
	in_quotes = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' && (i == 0 || str[i - 1] != '\\'))
			in_quotes = !in_quotes;
		if (str[i] != '|' && str[i] != ' ' && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if ((str[i] == '|' && !in_quotes) || str[i] == ' ')
			in_word = 0;
		i++;
	}
	return (count);
}

static void	free_result(char **result, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static void	handle_quotes(char c, int *in_quotes, char *quote_char)
{
	if (*in_quotes == 0)
	{
		*in_quotes = 1;
		*quote_char = c;
	}
	else if (c == *quote_char)
	{
		*in_quotes = 0;
		*quote_char = '\0';
	}
}

char	**split_cmds(char *input, t_shell *shell)
{
	char	**args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!input || input[0] == '\0' || is_only_spaces(input))
		return (NULL);
	if (check_syntax(input) || check_syntax_redir(input))
		return (NULL);
	args = malloc(sizeof(char *) * (count_words(input) + 1));
	if (!args)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!handle_segment(input, &args[j], &i))
		{
			ft_free_split(args);
			return (NULL);
		}
		if (args[j])
			j++;
		if (input[i] == '|')
			i++;
	}
	args[j] = NULL;
	return (args);
}

char	**ft_split_quotes(char *str)
{
	char	**result;
	char	*buffer;
	int		in_quotes;
	char	quote_char;
	int		i = 0, j = 0, start = 0, k;

	in_quotes = 0;
	quote_char = '\0';
	i = 0, j = 0, start = 0, k = 0;
	if (!str)
		return (NULL);
	result = malloc(sizeof(char *) * (count_words(str) + 2));
	while (str[i])
	{
		if (!in_quotes && (str[i] == '\'' || str[i] == '"'))
		{
			in_quotes = 1;
			quote_char = str[i++];
			start = i;
			while (str[i])
			{
				if (str[i] == '\\' && str[i + 1])
					i += 2;
				else if (str[i] == quote_char)
					break ;
				else
					i++;
			}
			buffer = ft_substr(str, start, i - start);
			result[k++] = buffer;
			if (str[i] == quote_char)
				i++;
		}
		else if (!in_quotes && str[i] != ' ')
		{
			start = i;
			while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '"')
				i++;
			buffer = ft_substr(str, start, i - start);
			result[k++] = buffer;
		}
		else
			i++;
	}
	result[k] = NULL;
	return (result);
}
