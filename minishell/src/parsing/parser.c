#include "../../includes/minishell.h"

static int	extract_word(char *str, char **result, int *count)
{
	char	*substr;

	substr = ft_substr(str, count[2], count[0] - count[2]);
	if (!substr)
	{
		while (--count[1] >= 0)
			free(result[count[1]]);
		free(result);
		return (0);
	}
	result[(count[1])++] = substr;
	count[3] = 0;
	return (1);
}

static void	process_non_space(char *str, int *pos, char *quote)
{
	if (!pos[3])
	{
		pos[2] = pos[0];
		pos[3] = 1;
	}
	if ((str[pos[0]] == '\'' || str[pos[0]] == '"')
		&& (pos[0] == 0 || str[pos[0] - 1] != '\\'))
	{
		if (!*quote)
			*quote = str[pos[0]];
		else if (str[pos[0]] == *quote)
			*quote = 0;
	}
}

static int	split_loop(char *str, char **result, int *pos)
{
	char	quote;

	quote = 0;
	while (str[pos[0]])
	{
		if ((str[pos[0]] == ' ' || str[pos[0]] == '\t') && !quote)
		{
			if (pos[3] && !extract_word(str, result, pos))
				return (0);
		}
		else
			process_non_space(str, pos, &quote);
		pos[0]++;
	}
	if (pos[3] && !extract_word(str, result, pos))
		return (0);
	return (1);
}

char	**ft_split_quotes(char *str)
{
	int		pos[4];
	char	**result;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char *) * (count_quote_words(str) + 1));
	if (!result)
		return (NULL);
	pos[0] = 0;
	pos[1] = 0;
	pos[3] = 0;
	if (!split_loop(str, result, pos))
		return (NULL);
	result[pos[1]] = NULL;
	return (result);
}
