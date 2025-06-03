#include "../../includes/minishell.h"

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
		if ((str[i] == '\'' || str[i] == '"') && (i == 0
				|| str[i - 1] != '\\'))
		{
			if (!quote)
				quote = str[i];
			else if (str[i] == quote)
				quote = 0;
			if (!in_word)
			{
				in_word = 1;
				count++;
			}
		}
		else if ((str[i] == ' ' || str[i] == '\t') && !quote)
		{
			in_word = 0;
		}
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}
