/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:20:55 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/23 14:20:36 by ddiogo-f         ###   ########.fr       */
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

void	delete_first_arg(char **args)
{
	int	i;

	i = 0;
	if (!args || !args[0])
		return ;
	free(args[0]);
	while (args[i + 1])
	{
		args[i] = args[i + 1];
		i++;
	}
	args[i] = NULL;
}

void	delete_empty_first_var_arg(t_command *cmd, char **envp, \
			int last_exit)
{
	char	*expanded;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (cmd->is_first_arg_var == 1 && cmd->args[0][1] != '\0')
	{
		expanded = expand_env_variable(cmd->args[0], envp, last_exit);
		if (!expanded || expanded[0] == '\0')
		{
			free(expanded);
			delete_first_arg(cmd->args);
			return ;
		}
		free(expanded);
	}
}
