#include "../includes/minishell.h"

int	is_unique_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || 
		!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"));
}

int	builtin_cd(char **args, bool has_fork)
{
	if (args[1])
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd\n");
			exit (EXIT_FAILURE);
		}
		if (has_fork)
			exit (EXIT_SUCCESS);
		return (0);
	}
	else
	{
		printf("cd: missing argument\n");// se nao tem args, vai para a root.
		if (has_fork)
			exit (EXIT_SUCCESS);
		return (0);
	}
}

void	builtin_exit(bool has_fork)
{
	// vericar o comportamento quando tem extras. 
	// ignoro? declaro erro e nao faz nada?
	if (!has_fork)
		//limpar tudo!!
	printf("exit\n");
	exit(EXIT_SUCCESS);
}

int builtin_export(char **args, t_shell *shell, bool has_fork)
{
	//nao faco ainda ideia do que isto vai fazer
	if (args && shell)
		return (0);
	if (has_fork)
		exit(EXIT_SUCCESS);
	return (0);
}

int	builtin_unset(char **args, t_shell *shell, bool has_fork)
{
	//nao faco ainda ideia do que isto vai fazer
	if (args && shell)
		return (0);
	if (has_fork)
		exit(EXIT_SUCCESS);
	return (0);
}