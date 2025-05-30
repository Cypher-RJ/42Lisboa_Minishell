#include "../includes/minishell.h"

int	is_unique_builtin(char *cmd)
{
	if (!cmd)
		return (1);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") \
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"));
}

int	builtin_cd(t_command *thiscmd, t_shell *shell, bool has_fork)
{
	int	i;

	i = 0;
	while (thiscmd->args[i])
		i++;
	if (i > 2)
		return (how_exit("minishell: cd: too many arguments", has_fork, \
			EXIT_FAILURE, shell));
	else if (i == 1 || !ft_strcmp(thiscmd->args[1], "~"))
		cd_go_home(shell, has_fork);
	else if (!ft_strcmp(thiscmd->args[1], "-"))
		cd_go_oldpwd(thiscmd, shell, has_fork);
	else
		return (builtin_cd_exec(thiscmd->args[1], shell, has_fork));
	return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
}

int	builtin_exit(t_command *thiscmd, t_shell *shell, bool has_fork)
{
	long long	status;

	status = EXIT_SUCCESS;
	ft_putendl_fd("exit", STDERR_FILENO);
	if (!thiscmd->args[1])
		how_exit(NULL, 1, EXIT_SUCCESS, shell);
	else if (!is_str_numeric(thiscmd->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(thiscmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		how_exit(NULL, 1, 2, shell);
	}
	else if (thiscmd->args[2])
		return (how_exit("minishell: exit: too many arguments", \
			has_fork, EXIT_FAILURE, shell));
	else
		ft_ms_atoll(thiscmd->args[1], &status);
	shell->exit_status = (unsigned char)status;
	return (how_exit(NULL, 1, (unsigned char)status, shell));
}

int	builtin_export(t_command *thiscmd, t_shell *shell, bool has_fork)
{
	int	i;
	int	valid;

	i = 1;
	valid = 0;
	if (!thiscmd->args[1])
		return (export_putenv(shell, has_fork));
	while (thiscmd->args[i])
	{
		if (!is_var_valid(thiscmd->args[i]))
			valid |= 1;
		else if (export_var(thiscmd->args[i], shell))
			return (how_exit(NULL, has_fork, EXIT_FAILURE, shell));
		i++;
	}
	return (how_exit(NULL, has_fork, valid, shell));
}

int	builtin_unset(t_command *thiscmd, t_shell *shell, bool has_fork)
{
	int	i;
	int	valid;

	i = 1;
	valid = 0;
	if (!thiscmd->args[1])
		return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
	while (thiscmd->args[i])
	{
		if (unset_var(thiscmd->args[i], shell))
			return (how_exit(NULL, has_fork, EXIT_FAILURE, shell));
		i++;
	}
	return (how_exit(NULL, has_fork, valid, shell));
}
