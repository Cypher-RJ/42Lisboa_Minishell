#include "../includes/minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = 128 + SIGINT;
		write(1, "\n", 1);
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
	int		exit_code = 0;

	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				if (sig == SIGINT)
					write(1, "\n", 1);
				else if (sig == SIGQUIT)
					write(1, "Quit (core dumped)\n", 19);
				exit_code = 128 + sig;
			}
		}
	}
	shell->exit_status = exit_code;
	g_signal_status = shell->exit_status;
}
