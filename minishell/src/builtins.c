#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "echo"));
}

int	how_exit(char *msg, bool has_fork, int out, t_shell *shell)
{
	shell->exit_status = out;
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	if (has_fork)
		exit (out);
	return (out);
}

void	builtin_pwd(t_command *thiscmd, t_shell *shell)
{
	char *cwd;

	cwd = NULL;
	if (thiscmd->args[1])
	{
		write(STDERR_FILENO, "pwd: too many arguments\n", 24);
		shell->exit_status = EXIT_FAILURE;
		exit(EXIT_FAILURE);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("Getcwd() failed to allocate\n");
		shell->exit_status = EXIT_FAILURE;
		exit(EXIT_FAILURE);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	shell->exit_status = EXIT_SUCCESS;
	exit (EXIT_SUCCESS);
}

void	builtin_echo(t_command *thiscmd, t_shell *shell)
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
		ft_putstr_fd(thiscmd->args[i], STDOUT_FILENO);
		if (thiscmd->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_nl)
		write(1, "\n", 1);
	shell->exit_status = EXIT_SUCCESS;
	exit (EXIT_SUCCESS);
}

void	builtin_env(t_command *cmds, t_shell *shell)
{
	int	i;

	i = 0;
	if (cmds->args[1])
	{
		ft_putendl_fd("env: too many arguments", STDERR_FILENO);
		shell->exit_status = EXIT_FAILURE;
		exit(EXIT_FAILURE);
	}
	while (shell->envp[i] && cmds)
	{
		ft_putendl_fd( shell->envp[i], STDOUT_FILENO);
		i++;
	}
	shell->exit_status = EXIT_SUCCESS;
	exit (EXIT_SUCCESS);
}
