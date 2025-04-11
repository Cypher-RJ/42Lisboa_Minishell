#include "minishell.h"

int	make_pipe(int fd[], bool has_next)
{
	if (has_next == 1)
	{
		if (pipe(fd) == -1)
			return (perror("Failed to create pipe"), -1);
		return (0);
	}
	else
		return (0);
}

pid_t	forkit()
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("Failed create a fork");
	return (pid);
}

void	child_pipes(int prev_fd, bool next, int fd[])
{
	int	res;

	res = 0;
	if (prev_fd != -1) // se nao for primeiro comando
	{
		res = dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
		if (res == -1)
		{
			perror("dup2 faile on prev_fd");
			exit(EXIT_FAILURE);
		}
	}
	if (next == 1) // se nao for ultimo comando
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (res == -1)
		{
			perror("dup2 failed on fd[1]");
			exit(EXIT_FAILURE);
		}
	}
}

void	executor(t_command **cmds, t_shell *shell)
{
	t_command	*thiscmd;
	int			fd[2];
	int			prev_fd;
	pid_t		pid;

	thiscmd = *cmds;
	prev_fd = -1;
	while (thiscmd != NULL)
	{
		if (make_pipe(fd, (thiscmd->next != NULL)) == -1)
			break;
		pid = fork_it();
		if (pid == -1)
			break;
		if (pid == 0) // Processo filho
		{
			child_pipes(prev_fd, (thiscmd->next != NULL), fd); // se falhar faz exit da child, sem espinhas
			if (thiscmd->redir) // se ha redir, sobrepoe o que foi feito aos pipes
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
