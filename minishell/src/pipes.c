#include "minishell.h"

// integrar esta função com o que ja está dos pipes
void	execute_pipe(t_command **cmds, t_shell *shell)
{
	t_command	*thiscmd;
	int		fd[2];
	int		prev_fd;
	pid_t		pid;

	thiscmd = *cmds;
	prev_fd = -1;
	while (thiscmd != NULL)
	{
		if (thiscmd->next != NULL)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) // Processo filho
		{
			if (prev_fd != -1) // se nao for primeiro comando
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (thiscmd->next != NULL) // se nao for ultimo comando
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}
			if (thiscmd->redir) // se ha redir, sobrepor o que foi feito aos pipes
				redirector(&thiscmd->redir);
			if (is_builtin(thiscmd->args[0]) == 1)
				execute_builtin(thiscmd->args, shell);
			else
				execute_command(thiscmd, shell->envp);
		}
		// Processo pai
		if (prev_fd != -1)
			close(prev_fd);
		if (thiscmd->next != NULL)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		thiscmd = thiscmd->next;
	}
	// Esperar pelos filhos
	thiscmd = *cmds;
	while (thiscmd != NULL)
	{
		wait(NULL);
		thiscmd = thiscmd->next;
	}
}
