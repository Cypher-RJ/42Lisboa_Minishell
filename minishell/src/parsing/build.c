/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:26:16 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/23 12:01:20 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**process_args(char **args, t_shell *shell, int *is_first_arg_var)
{
	char	*expanded;
	char	*temp;
	int		j;

	if (args[0] && args[0][0] == '$' && args[0][1] != '\0')
		*is_first_arg_var = 1;
	else
		*is_first_arg_var = 0;
	j = 0;
	while (args[j])
	{
		temp = args[j];
		expanded = expand_env_variable(args[j], shell->envp, \
			shell->exit_status);
		if (!expanded)
			return (NULL);
		args[j] = remove_outer_quotes(expanded);
		if (temp != args[j])
			free(temp);
		j++;
	}
	return (args);
}

static t_command	*create_new_command(char **args, int is_first_arg_var)
{
	t_command	*new_node;

	new_node = ft_calloc(1, sizeof(t_command));
	if (!new_node)
		return (NULL);
	new_node->args = args;
	new_node->next = NULL;
	new_node->is_first_arg_var = is_first_arg_var;
	return (new_node);
}

static int	process_command(char *cmd, t_shell *shell, t_command **head, \
	t_command **current)
{
	char		**args;
	t_command	*new_node;
	int			is_first_arg_var;

	args = ft_split_quotes(cmd);
	if (!args)
		return (0);
	if (!process_args(args, shell, &is_first_arg_var))
	{
		ft_free_split(args);
		return (0);
	}
	new_node = create_new_command(args, is_first_arg_var);
	if (!new_node)
	{
		ft_free_split(args);
		return (0);
	}
	if (!*head)
		*head = new_node;
	else
		(*current)->next = new_node;
	*current = new_node;
	return (1);
}

t_command	*build_command_list(char **cmds, t_shell *shell)
{
	t_command	*head;
	t_command	*current;
	int			i;

	head = NULL;
	current = NULL;
	i = 0;
	if (!cmds || !cmds[0])
		return (NULL);
	while (cmds[i])
	{
		if (!process_command(cmds[i], shell, &head, &current))
		{
			free_commands(head);
			return (NULL);
		}
		i++;
	}
	return (head);
}

t_command	*build_redir(t_command *cmds)
{
	t_redirect	*redir_head;
	t_redirect	*redir_tail;
	int			i;

	if (!cmds)
		return (NULL);
	redir_head = NULL;
	redir_tail = NULL;
	i = 0;
	while (cmds->args && cmds->args[i])
	{
		if (cmds->args[i] && is_redirect_op(cmds->args[i]))
		{
			if (!process_redirect(cmds, i, &redir_head, &redir_tail))
			{
				free_redirect_list(redir_head);
				return (NULL);
			}
			continue ;
		}
		i++;
	}
	cmds->redir = redir_head;
	return (cmds);
}
