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

int	builtin_exit(char **args, t_shell *shell, bool has_fork)
{
	long long status;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!args[1])
		status = EXIT_SUCCESS;
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		status = 1;
	}
	else if (!is_str_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		status = 255;
	}
	else
		status = ft_atoll(args[1]);
	shell->exit_status = status;
	if (!has_fork)
		//limpar tudo!!
	exit(status);
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