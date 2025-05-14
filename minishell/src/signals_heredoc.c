#include "../includes/minishell.h"

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(0); // Fecha STDIN para forçar readline a retornar NULL
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
