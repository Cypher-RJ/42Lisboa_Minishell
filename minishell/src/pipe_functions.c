/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:20:13 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/06 09:20:23 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	make_pipe(int fd[], bool has_next)
{
	if (has_next == 1)
	{
		if (pipe(fd) == -1)
			return (perror("Failed to create pipe"), -1);
		return (0);
	}
	else
	{
		return (0);
	}
}

pid_t	fork_it(void)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		perror("Failed create a fork");
	return (pid);
}

void	child_pipes(int prev_fd, bool next, int fd[], t_shell *shell)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			close(prev_fd);
			close(fd[0]);
			close(fd[1]);
			how_exit("faile on prev_fd", 1, EXIT_FAILURE, shell);
		}
		close(prev_fd);
	}
	if (next == 1)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			how_exit("dup2 failed on fd[1]", 1, EXIT_FAILURE, shell);
		}
		close(fd[0]);
		close(fd[1]);
	}
}
