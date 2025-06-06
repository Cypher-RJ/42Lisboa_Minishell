/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:27:27 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:27:28 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redir_list(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->direction);
		redir->direction = NULL;
		free(redir->passorfile);
		redir->passorfile = NULL;
		if (redir->hf_fd != -1)
			close(redir->hf_fd);
		free(redir);
		redir = tmp;
	}
}

void	free_command_list(t_command *cmds)
{
	t_command	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
		{
			ft_free_split(cmds->args);
			cmds->args = NULL;
		}
		if (cmds->path)
		{
			free(cmds->path);
			cmds->path = NULL;
		}
		if (cmds->redir)
		{
			free_redir_list(cmds->redir);
			cmds->redir = NULL;
		}
		free(cmds);
		cmds = tmp;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->envp)
	{
		ft_free_split(shell->envp);
		shell->envp = NULL;
	}
	free(shell);
	rl_clear_history();
}

void	free_total(t_shell *shell)
{
	if (shell->cmds)
	{
		free_command_list(shell->cmds);
		shell->cmds = NULL;
	}
	if (shell)
		free_shell(shell);
}
