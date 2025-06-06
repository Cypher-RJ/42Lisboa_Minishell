/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:28:04 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:28:05 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = 128 + SIGINT;
		write(1, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	pass_signal_status(t_shell	*shell)
{
	shell->exit_status = g_signal_status;
	g_signal_status = 0;
}

void	restore_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	wait_for_children(pid_t last_pid, t_shell *shell)
{
	int		status;
	pid_t	pid;
	int		exit_code;
	int		ctrlc;

	exit_code = 0;
	ctrlc = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
			exit_code = signal_fork(status, &ctrlc);
		else
			signal_fork(status, &ctrlc);
		pid = wait(&status);
	}
	shell->exit_status = exit_code;
	setup_signals();
}
