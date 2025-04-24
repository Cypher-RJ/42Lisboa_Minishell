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

void	builtin_echo(t_command *thiscmd)
{
	int		i;
	bool	no_nl;

	i = 1;
	no_nl = false;
	if (thiscmd->args[i] && !(ft_strcmp(thiscmd->args[i], "-n")))
	{
		no_nl = true;
		i++;
	}
	while (thiscmd->args[i])
	{
		write(1 , thiscmd->args[i], ft_strlen(thiscmd->args[i]));
		if (thiscmd->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_nl)
		write(1, "\n", 1);
	exit (EXIT_SUCCESS);
}

void	builtin_env(t_command *cmds, t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i] && cmds)
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	exit (EXIT_SUCCESS);
}
