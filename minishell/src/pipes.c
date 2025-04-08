#include "minishell.h"

void	execute_pipe(char **cmd1, char **cmd2, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		return ;
	}
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (is_builtin(cmd1[0]))
			execute_builtin(cmd1, envp);
		else
			execve(get_path(cmd1[0], envp), cmd1, envp);
		perror("execve");
		exit(1);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		return ;
	}
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (is_builtin(cmd2[0]))
			execute_builtin(cmd2, envp);
		else
			execve(get_path(cmd2[0], envp), cmd2, envp);
		perror("execve");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

// integrar esta função com o que ja está dos pipes
void	execute_pipeline(t_command **cmds, t_shell *shell)
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
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->next != NULL)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}
			// isto tem que ser adaptado para usar
			//
			if (execve(cmd->path, cmd->args, shell->envp) ==  -1)
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
