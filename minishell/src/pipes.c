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
		execve(get_path(cmd2[0], envp), cmd2, envp);
		perror("execve");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
