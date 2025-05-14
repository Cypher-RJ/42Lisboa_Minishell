#include "../../includes/minishell.h"

static int	count_words(char *str)
{
	int	count;
	int	in_word;
	int	in_quotes;
	int	i;
	char quote_char;

	count = 0;
	in_word = 0;
	in_quotes = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && (i == 0 || str[i - 1] != '\\'))
		{
			if (!in_quotes)
			{
				quote_char = str[i];
				in_quotes = 1;
				if (in_word == 0)
				{
					in_word = 1;
					count++;
				}
			}
			else if (str[i] == quote_char)
			{
				in_quotes = 0;
			}
		}
		else if (str[i] != '|' && str[i] != ' ' && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (!in_quotes && ((str[i] == '|') || str[i] == ' '))
			in_word = 0;
		i++;
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
			quote = str[i++];
			start = i;
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
			{
				result[k++] = ft_substr(str, start, i - start);
				i++;
			}
			else
				result[k++] = ft_substr(str, start - 1, i - start + 1);
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






//! ? depois do $
//! 