#include "minishell.h"

static char *get_path(char *cmd)
{
	char *path = getenv("PATH");
	char **paths;
	char *full_path;
	int i = 0;

	if(!path || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));

	paths = ft_split(path, ':');
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void execute_command(char **args, char **envp)
{
	pid_t pid;
	char *full_path;

	if (!args[0])
		return ;

	full_path = get_path(args[0]);
	if (!full_path)
	{
		printf("minishell: command not found: %s\n", args[0]);
		return ;
	}

	pid = fork();
	if(pid == 0)
	{
		if (execve(full_path, args, envp) == -1)
			perror("execve");
		exit(1);
	}
	else if (pid > 0)
		wait(NULL);
	else
		perror("fork");
}

int is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") ||
			!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
			!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
			!ft_strcmp(cmd, "echo"));
}

void execute_builtin(char **args)
{
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
		int i = 1;
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
		printf("Exiting minishell...\n");
		exit(0);
	}
}