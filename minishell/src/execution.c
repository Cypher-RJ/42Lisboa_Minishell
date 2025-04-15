#include "../includes/minishell.h"

void	execute_command(t_command *cmd, t_shell *shell)
{
	if (execve(cmd->path, cmd->args, shell->envp) == -1)
	{
		perror("execve");
		exit(1);
	}
}

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "echo"));
}

void	execute_builtin(char **args, t_shell *shell)
{//colocar um exit sucess ou error em todos os comandos
	int	i;

	if (!ft_strcmp(args[0], "cd"))
	{
		if (args[1])
		{
			if (chdir(args[1]) != 0)
			{
				perror("cd");
				exit (EXIT_FAILURE);
			}
			exit (EXIT_SUCCESS);
		}
		else
		{
			printf("cd: missing argument\n");// se nao tem args, vai para a root.
			exit (EXIT_SUCCESS); 
		}
	}
	else if (!ft_strcmp(args[0], "pwd"))
	{
		printf("%s\n", getcwd(NULL, 0));
		exit (EXIT_SUCCESS);
	}
	else if (!ft_strcmp(args[0], "echo"))
	{
		i = 1;
		while (args[i])// falta ver se tem -n(nao mete \n no fim)
		{
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" "); // acho que o echo transforma todos os argumentos numa unica string
			i++;
		}
		printf("\n");
	}
	else if (!ft_strcmp(args[0], "env"))
	{
		i = 0;
		while (shell->envp[i])
		{
			printf("%s\n", shell->envp[i]);
			i++;
		}
		exit (EXIT_SUCCESS);
	}
	else if (!ft_strcmp(args[0], "exit")) // isto precisa de verificar se so tem uma palavra. Acho que se tiver mais passa tudo a string
	{
		printf("exit\n");//e suposto escrever algo?
		exit(EXIT_SUCCESS);
	}
}
