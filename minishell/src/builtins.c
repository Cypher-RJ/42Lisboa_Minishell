#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "echo"));
}

void	builtin_pwd()
{
	ft_printf("%s\n", getcwd(NULL, 0));
	exit (EXIT_SUCCESS);
}

void	builtin_echo(char **args)
{
	int	i;

	i = 1;
	while (args[i])// falta ver se tem -n(nao mete \n no fim)
	{
		write(1 , args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1); // acho que o echo transforma todos os argumentos numa unica string
		i++;
	}
	write(1, "\n", 1);
	exit (EXIT_SUCCESS);
}

void	builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	exit (EXIT_SUCCESS);
}
