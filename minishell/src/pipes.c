#include "minishell.h"

// integrar esta função com o que ja está dos pipes
void	execute_pipe(t_command **cmds, t_shell *shell)
{
	t_command	*cmd;
	int		fd[2];
	int		prev_fd;
	pid_t		pid;

	cmd = *cmds;
	prev_fd = -1;
	while (cmd != NULL)
	{
		if (cmd->next != NULL)
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
			if (cmd->next != NULL) // se nao for ultimo comando
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}
			if (cmd->redir) // se ha redir, sobrepor o que foi feito aos pipes
				redirector(&cmd->redir);
			if (execve(cmd->path, cmd->args, shell->envp) == -1)
			{
				perror("execvp");
				exit(EXIT_FAILURE);
			}
		}
		// Processo pai
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next != NULL)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		cmd = cmd->next;
	}
	// Esperar pelos filhos
	cmd = *cmds;
	while (cmd != NULL)
	{
		wait(NULL);
		cmd = cmd->next;
	}
}
