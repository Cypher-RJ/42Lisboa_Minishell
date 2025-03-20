#include "minishell.h"

void execute_pipe(char **cmd1, char **cmd2, char **envp)
{
	int fd[2];
	pid_t pid1, pid2;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return ;
	}
	
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve(get_path(cmd1[0], envp), cmd1, envp);
		perror("execve");
		exit(1);
	}
	if (pid2 == 0)
	{
		dup2(fd[0], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve(get_path(cmd2[0], envp), cmd2, envp);
		perror("execve");
		exit(1);
	}

	close(fd[0]);
	close(fd[1]);
	wait(NULL);
	wait(NULL);
}