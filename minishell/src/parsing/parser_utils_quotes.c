#include "../../includes/minishell.h"

static void	handle_quote_opening(char c, int *in_word, char *quote, int *ct)
{
	if (!*quote)
	{
		*quote = c;
		if (!*in_word)
		{
			*in_word = 1;
			(*ct)++;
		}
	}
	else if (c == *quote)
		*quote = 0;
}

int	count_quote_words(char *str)
{
	int		count;
	int		i;
	int		in_word;
	char	quote;

	count = 0;
	i = 0;
	in_word = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"')
			&& (i == 0 || str[i - 1] != '\\'))
			handle_quote_opening(str[i], &in_word, &quote, &count);
		else if ((str[i] == ' ' || str[i] == '\t') && !quote)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}
