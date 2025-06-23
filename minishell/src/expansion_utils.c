/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 09:34:08 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/17 18:13:38 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	has_unquoted_space(const char *str)
{
	int	in_single;
	int	in_double;
	int	i;

	in_double = 0;
	in_single = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == ' ' && !in_single && !in_double)
			return (1);
		i++;
	}
	return (0);
}

void	check_and_split_expanded_command(t_command *cmd)
{
	char	**split;
	int		i;
	char	*arg;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	arg = cmd->args[0];
	// Only split if the argument is a pure variable expansion (starts with $ and is not just $ itself)
	if (arg[0] == '$' && arg[1] && has_unquoted_space(arg))
	{
		split = ft_split_quotes(arg);
		if (!split)
			return ;
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		cmd->args = split;
	}
}
