#include "minishell.h"

static int	handle_segment(char *input, char **arg_slot, int *i, t_shell *shell)
{
	int	start;
	int	in_quotes;

	in_quotes = 0;
	while (input[*i] == ' ')
		(*i)++;
	start = *i;
	while (input[*i])
	{
		if (input[*i] == '"' && (*i == 0 || input[*i - 1] != '\\'))
			in_quotes = !in_quotes;
		else if (input[*i] == '|' && !in_quotes)
			break ;
		(*i)++;
	}
	if (start == *i)
		return (1);
	*arg_slot = ft_substr(input, start, *i - start);
	if (!*arg_slot)
		return (0);
	*arg_slot = expand_env_variable(*arg_slot, shell->envp);
	return (*arg_slot != NULL);
}

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

/* char	***split_command(char *input, char **envp)
{
	char	***args;
	int		i;
	int		j;
	int		start;
	int		word_count;
	char	**args;
	int		i;
	int		j;

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
			if (!args[j])
			{
				ft_free_split(args);
				return (NULL);
			}
			j++;
		}
	}
	args[j] = NULL;
	return (args);
} */
char	**split_cmds(char *input, t_shell *shell)
{
	char	**args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	args = malloc(sizeof(char *) * (count_words(input) + 1));
	if (!args)
		return (NULL);
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
