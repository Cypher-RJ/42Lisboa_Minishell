/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darkless12 <darkless12@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:20:27 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/11 14:21:28 by darkless12       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_labor(int prev_fd, t_command *thiscmd, int fd[], t_shell *shell)
{
	restore_signals();
	child_pipes(prev_fd, (thiscmd->next != NULL), fd, shell);
	redirector(thiscmd->redir, shell, 1);
	if (thiscmd->args[0] == NULL)
		how_exit(NULL, 1, EXIT_SUCCESS, shell);
	else
	{
		execute_builtin(thiscmd, shell, 1);
		execute_command(thiscmd, shell, 1);
	}
}

int	daddy_time(int *prev_fd, bool next, int fd[])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (next == 1)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	return (*prev_fd);
}

void	executor_fork(t_shell *shell)
{
	t_command	*thiscmd;
	int			fd[2];
	int			prev_fd;
	pid_t		pid;

	thiscmd = shell->cmds;
	prev_fd = -1;
	while (thiscmd != NULL)
	{
		if (make_pipe(fd, (thiscmd->next != NULL)) == -1)
			break ;
		pid = fork_it();
		if (pid == -1)
			break ;
		if (pid == 0)
			child_labor(prev_fd, thiscmd, fd, shell);
		prev_fd = daddy_time(&prev_fd, (thiscmd->next != NULL), fd);
		thiscmd = thiscmd->next;
	}
	wait_for_children(pid, shell);
}
