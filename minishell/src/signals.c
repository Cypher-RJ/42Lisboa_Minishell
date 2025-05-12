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
