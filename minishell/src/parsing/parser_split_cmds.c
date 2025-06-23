/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split_cmds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:26:28 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/17 13:47:18 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
