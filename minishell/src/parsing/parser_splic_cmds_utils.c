/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_splic_cmds_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darkless12 <darkless12@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:26:20 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/07/25 19:05:07 by darkless12       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	should_add_space(const char *input, int i, int pos)
{
	if (pos == 0)
		return (i > 0 && input[i - 1] != ' ' && input[i - 1] != '<' \
&& input[i - 1] != '>');
	else
		return (input[i + 1] && input[i + 1] != ' ' && input[i + 1] != '<' \
&& input[i + 1] != '>');
}

static int	parse_handle_redir(const char *input, char *new_str, int *i, int *j)
{
	if (should_add_space(input, *i, 0))
		new_str[(*j)++] = ' ';
	new_str[(*j)++] = input[*i];
	if (input[*i + 1] == input[*i])
		new_str[(*j)++] = input[++(*i)];
	if (should_add_space(input, *i, 1))
		new_str[(*j)++] = ' ';
	return (1);
}

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

static void	handle_pipe(const char *input, char *new_str, int i, int *j)
{
	if (should_add_space(input, i, 0))
		new_str[(*j)++] = ' ';
	new_str[(*j)++] = '|';
	if (should_add_space(input, i, 1))
		new_str[(*j)++] = ' ';
}

char	*add_spaces_around_redir(const char *input)
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
			parse_handle_redir(input, new_str, &i, &j);
		else if (!in_quote && input[i] == '|')
			handle_pipe(input, new_str, i, &j);
		else
			new_str[j++] = input[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}
