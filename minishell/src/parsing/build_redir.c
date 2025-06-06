/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:26:08 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:26:09 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redirect_list(t_redirect *redir_head)
{
	t_redirect	*temp;

	while (redir_head)
	{
		temp = redir_head;
		redir_head = redir_head->next;
		free(temp->direction);
		free(temp->passorfile);
		free(temp);
	}
}

static t_redirect	*create_redirect(char *direction, char *file)
{
	t_redirect	*redirect;

	redirect = malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->direction = ft_strdup(direction);
	redirect->passorfile = ft_strdup(file);
	redirect->hf_fd = -1;
	redirect->next = NULL;
	if (!redirect->direction || !redirect->passorfile)
	{
		free(redirect->direction);
		free(redirect->passorfile);
		free(redirect);
		return (NULL);
	}
	return (redirect);
}

int	is_redirect_op(char *str)
{
	return (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0
		|| ft_strcmp(str, "<<") == 0 || ft_strcmp(str, ">>") == 0);
}

static void	shift_args_left(char **args, int start)
{
	int	j;

	j = start;
	while (args[j + 2])
	{
		args[j] = args[j + 2];
		j++;
	}
	args[j] = NULL;
	args[j + 1] = NULL;
}

int	process_redirect(t_command *cmds, int i, t_redirect **redir_head,
		t_redirect **redir_tail)
{
	t_redirect	*redirect;

	if (!cmds->args[i + 1])
		return (0);
	redirect = create_redirect(cmds->args[i], cmds->args[i + 1]);
	if (!redirect)
		return (0);
	if (!*redir_head)
		*redir_head = redirect;
	else
		(*redir_tail)->next = redirect;
	*redir_tail = redirect;
	free(cmds->args[i]);
	free(cmds->args[i + 1]);
	shift_args_left(cmds->args, i);
	return (1);
}
