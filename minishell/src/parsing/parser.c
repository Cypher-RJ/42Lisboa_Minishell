#include "../../includes/minishell.h"

static int	count_words(char *str)
{
	int		count;
	int		i;
	int		in_word;
	char	quote_type;

	count = 0;
	i = 0;
	in_word = 0;
	quote_type = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && (i == 0 || str[i - 1] != '\\'))
		{
			if (!quote_type)
			{
				if (!in_word)
				{
					in_word = 1;
					count++;
				}
				quote_type = str[i];
			}
			else if (str[i] == quote_type)
			{
				quote_type = 0;
			}
		}
		else if (str[i] == ' ' && !quote_type)
		{
			in_word = 0;
		}
		else if (!in_word && !quote_type)
		{
			in_word = 1;
			count++;
		}
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
	int		i;
	int		k;
	int		start;
	char	**result;
	char	in_quote;
	int		word_count;
	int		in_word;
	int		j;
	char	*substr;

	i = 0;
	k = 0;
	in_quote = 0;
	if (!str)
		return (NULL);
	word_count = 0;
	in_word = 0;
	j = 0;
	while (str[j])
	{
		if ((str[j] == '\'' || str[j] == '"') && (j == 0 || str[j - 1] != '\\'))
		{
			if (!in_quote)
			{
				if (!in_word)
				{
					in_word = 1;
					word_count++;
				}
				in_quote = str[j];
			}
			else if (str[j] == in_quote)
			{
				in_quote = 0;
				in_word = 0;
				if (str[j + 1] && str[j + 1] != ' ' && str[j + 1] != '\t')
				{
					word_count++;
					in_word = 1;
				}
			}
		}
		else if ((str[j] == ' ' || str[j] == '\t') && !in_quote)
		{
			if (in_word)
				in_word = 0;
		}
		else if (!in_word)
		{
			in_word = 1;
			word_count++;
		}
		j++;
	}
	result = malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	in_word = 0;
	in_quote = 0;
	while (str[i])
	{
		if ((str[i] == ' ' || str[i] == '\t') && !in_quote)
		{
			if (in_word)
			{
				substr = ft_substr(str, start, i - start);
				if (!substr)
				{
					while (--k >= 0)
						free(result[k]);
					free(result);
					return (NULL);
				}
				result[k++] = substr;
				in_word = 0;
			}
			i++;
			continue ;
		}
		if (!in_word)
		{
			start = i;
			in_word = 1;
		}
		if ((str[i] == '\'' || str[i] == '"') && (i == 0 || str[i - 1] != '\\'))
		{
			if (!in_quote)
				in_quote = str[i];
			else if (str[i] == in_quote)
				in_quote = 0;
		}
		i++;
		if (!str[i] && in_word)
		{
			substr = ft_substr(str, start, i - start);
			if (!substr)
			{
				while (--k >= 0)
					free(result[k]);
				free(result);
				return (NULL);
			}
			result[k++] = substr;
		}
	}
	result[k] = NULL;
	return (result);
}
