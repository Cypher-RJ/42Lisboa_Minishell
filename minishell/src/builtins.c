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
	int	i;

	i = 1;
	while (thiscmd->args[i])// falta ver se tem -n(nao mete \n no fim)
	{
		write(1 , thiscmd->args[i], ft_strlen(thiscmd->args[i]));
		if (thiscmd->args[i + 1])
			write(1, " ", 1);
		i++;
	}
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
