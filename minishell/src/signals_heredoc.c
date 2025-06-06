/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:20:55 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/06 09:21:04 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_signal_status = 1;
	write(1, "\n", 1);
	close(0);
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	signal_fork(int status, int *ctrlc)
{
	int		exit_code;
	int		sig;

	exit_code = 0;
	sig = 0;
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT && *ctrlc == 0)
		{
			write(1, "^C\n", 3);
			*ctrlc = 1;
		}
		else if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		exit_code = 128 + sig;
	}
	return (exit_code);
}
