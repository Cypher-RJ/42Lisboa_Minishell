#include "minishell.h"

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

char	**split_cmds(char *input, t_shell *shell)
{
	char **args;
	int i;
	int j;
	
	i = 0;
	j = 0;
	if (!input || input[0] == '\0' || is_only_spaces(input))
		return (NULL);
	if (check_syntax(input))
		return (NULL);
	args = malloc(sizeof(char *) * (count_words(input) + 1));
	if (!args)
	return (NULL);
	ft_memset(args, 0, sizeof(char *) * (count_words(input) + 1));
	while (input[i])
	{
		if (!handle_segment(input, &args[j], &i, shell))
		return (ft_free_split(args), NULL);
		if (args[j])
		j++;
		if (input[i] == '|')
		i++;
	}
	args[j] = NULL;
	return (args);
}
