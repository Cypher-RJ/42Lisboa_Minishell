#include "minishell.h"

static int	count_words(char *str)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != ' ' && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == ' ')
			in_word = 0;
		str++;
	}
	return (count);
}

char	**split_command(char *input, char **envp)
{
	char	**args;
	int		i;
	int		j;
	int		start;
	int		word_count;

	i = 0;
	j = 0;
	word_count = count_words(input);
	args = malloc(sizeof(char *) * (word_count + 1));
	if (!args)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		start = i;
		while (input[i] && input[i] != ' ')
			i++;
		if (start != i)
		{
			args[j] = expand_env_variable(ft_substr(input, start, i - start),
					envp);
			j++;
		}
	}
	args[j] = NULL;
	return (args);
}
