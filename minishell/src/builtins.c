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
		ft_putendl_fd(msg, STDERR_FILENO);
	if (has_fork)
	{
		free_total(shell);
		exit (out);
	}
	return (out);
}

void	builtin_pwd(t_command *thiscmd, t_shell *shell)
{
	char *cwd;

	cwd = NULL;
	if (thiscmd->args[1])
		how_exit("pwd: too many arguments", 1, EXIT_FAILURE, shell);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		how_exit("failed to allocate\n", 1, EXIT_FAILURE, shell);
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	how_exit(NULL, 1, EXIT_SUCCESS, shell);
}

void	builtin_echo(t_command *thiscmd, t_shell *shell)
{
	int		i;
	bool	no_nl;

	i = 1;
	no_nl = false;
	if (thiscmd->args[i] && !(ft_strcmp(thiscmd->args[i++], "-n")))
		no_nl = true;
	while (thiscmd->args[i])
	{
		ft_putstr_fd(thiscmd->args[i], STDOUT_FILENO);
		if (thiscmd->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_nl)
		write(1, "\n", 1);
	how_exit(NULL, 1, EXIT_SUCCESS, shell);
}

void	builtin_env(t_command *cmds, t_shell *shell)
{
	int	i;

	i = 0;
	if (cmds->args[1])
		how_exit("env: too many arguments", 1, EXIT_FAILURE, shell);
	while (shell->envp[i] && cmds)
	{
		ft_putendl_fd( shell->envp[i], STDOUT_FILENO);
		i++;
	}
	how_exit(NULL, 1, EXIT_SUCCESS, shell);
}
