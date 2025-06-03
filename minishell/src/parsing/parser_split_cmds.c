#include "../../includes/minishell.h"

static void	handle_quote_state(char c, int *in_quote, int i, const char *input)
{
	if ((c == '\'' || c == '"') && (i == 0 || input[i - 1] != '\\'))
	{
		if (!*in_quote)
			*in_quote = c;
		else if (*in_quote == c)
			*in_quote = 0;
	}
}

static int	should_add_space_before(const char *input, int i)
{
	return (i > 0 && input[i - 1] != ' ' && input[i - 1] != '<'
			&& input[i - 1] != '>');
}

static int	should_add_space_after(const char *input, int i)
{
	return (input[i + 1] && input[i + 1] != ' ' && input[i + 1] != '<'
			&& input[i + 1] != '>');
}

static int	handle_redirection(const char *input, char *new_str, int *i, int *j)
{
	if (should_add_space_before(input, *i))
		new_str[(*j)++] = ' ';
	new_str[(*j)++] = input[*i];
	if (input[*i + 1] == input[*i])
		new_str[(*j)++] = input[++(*i)];
	if (should_add_space_after(input, *i))
		new_str[(*j)++] = ' ';
	return (1);
}

static char	*add_spaces_around_redir(const char *input)
{
	int		i;
	int		j;
	int		in_quote;
	char	*new_str;

	i = 0;
	j = 0;
	in_quote = 0;
	new_str = malloc(ft_strlen(input) * 3 + 1);
	if (!new_str)
		return (NULL);
	while (input[i])
	{
		handle_quote_state(input[i], &in_quote, i, input);
		if (!in_quote && (input[i] == '<' || input[i] == '>'))
			handle_redirection(input, new_str, &i, &j);
		else
			new_str[j++] = input[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

static void	handle_quote_counting(char c, char *quote_type, int *in_word,
		int *count)
{
	if (!*quote_type)
	{
		if (!*in_word)
		{
			*in_word = 1;
			(*count)++;
		}
		*quote_type = c;
	}
	else if (c == *quote_type)
		*quote_type = 0;
}

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
		if ((str[i] == '\'' || str[i] == '"') && (i == 0
				|| str[i - 1] != '\\'))
			handle_quote_counting(str[i], &quote_type, &in_word, &count);
		else if (str[i] == ' ' && !quote_type)
			in_word = 0;
		else if (!in_word && !quote_type)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

static char	**allocate_args(char *pre)
{
	char	**args;

	args = malloc(sizeof(char *) * (count_words(pre) + 1));
	if (!args)
	{
		free(pre);
		return (NULL);
	}
	return (args);
}

static int	parse_input(char *pre, char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (pre[i])
	{
		while (pre[i] == ' ')
			i++;
		if (!handle_segment(pre, &args[j], &i))
			return (0);
		if (args[j])
			j++;
		if (pre[i] == '|')
			i++;
	}
	args[j] = NULL;
	return (1);
}

char	**split_cmds(char *input)
{
	char	*pre;
	char	**args;

	pre = add_spaces_around_redir(input);
	if (!pre || pre[0] == '\0' || is_only_spaces(pre))
		return (free(pre), NULL);
	if (check_syntax(pre) || check_syntax_redir(pre))
		return (free(pre), NULL);
	args = allocate_args(pre);
	if (!args)
		return (NULL);
	if (!parse_input(pre, args))
	{
		ft_free_split(args);
		free(pre);
		return (NULL);
	}
	free(pre);
	return (args);
}
