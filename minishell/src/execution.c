#include "minishell.h"

int	has_pipe(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	execute_command(char **args, char **envp)
{
	int		pipe_index;
	int		fd_in;
	int		fd_out;
	pid_t	pid;

	fd_in = -1;
	pipe_index = has_pipe(args);
	if (pipe_index != -1)
	{
		args[pipe_index] = NULL;
		execute_pipe(args, &args[pipe_index + 1], envp);
		return ;
	}
	fd_in = -1;
	fd_out = -1;
	if (detect_redirections(args, &fd_in, &fd_out))
		return ;
	pid = fork();
	if (pid == 0)
	{
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
		execve(get_path(args[0], envp), args, envp);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
		wait(NULL);
	else
		perror("fork");
}

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "echo"));
}

void	execute_builtin(char **args)
{
	int	i;

	if (!ft_strcmp(args[0], "cd"))
	{
		if (args[1])
		{
			if (chdir(args[1]) != 0)
				perror("cd");
		}
		else
			printf("cd: missing argument\n");
	}
	else if (!ft_strcmp(args[0], "pwd"))
		printf("%s\n", getcwd(NULL, 0));
	else if (!ft_strcmp(args[0], "echo"))
	{
		i = 1;
		while (args[i])
		{
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" ");
			i++;
		}
		printf("\n");
	}
	else if (!ft_strcmp(args[0], "exit"))
	{
		printf("exit\n");
		exit(0);
	}
}
