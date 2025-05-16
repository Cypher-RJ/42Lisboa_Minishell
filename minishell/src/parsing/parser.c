#include "../../includes/minishell.h"

static int	count_words(char *str)
{
	int count = 0;
	int i = 0;

	while (str[i])
	{
		// Skip spaces
		while (str[i] == ' ')
			i++;

		if (!str[i])
			break;

		// Handle a quoted segment
		if (str[i] == '\'' || str[i] == '"')
		{
			char quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
				i++;
			count++;
		}
		else
		{
			// Regular word (unquoted)
			while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'')
				i++;
			count++;
		}
	}
	return (count);
}

char	**split_cmds(char *input)
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
	int		i = 0, k = 0, start;
	char	**result;
	char	quote;
	int		word_count;

	word_count = count_words(str);
	result = malloc(sizeof(char *) * (word_count + 1));
	if (!str || !result)
		return (NULL);

	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break;

		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			start = i; // include the quote
			i++;
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
			{
				i++; // include the closing quote
				result[k++] = ft_substr(str, start, i - start);
			}
			else
			{
				// Unterminated quote: take until end
				result[k++] = ft_substr(str, start, i - start);
			}
		}
		else
		{
			start = i;
			while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '"')
				i++;
			result[k++] = ft_substr(str, start, i - start);
		}
	}
	result[k] = NULL;
	return (result);
}
