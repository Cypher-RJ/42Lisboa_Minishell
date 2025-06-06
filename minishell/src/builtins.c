/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:17:17 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/06 09:17:28 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	g_signal_status = shell->exit_status;
	if (msg)
		ft_putendl_fd(msg, STDERR_FILENO);
	if (has_fork)
	{
		free_total(shell);
		exit (out);
	}
	return (out);
}

void	builtin_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = NULL;
	cwd = get_pwd(shell);
	if (!cwd)
		how_exit("failed to allocate to PWD", 1, EXIT_FAILURE, shell);
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	how_exit(NULL, 1, EXIT_SUCCESS, shell);
}

void	builtin_echo(t_command *thiscmd, t_shell *shell)
{
	int		i;
	int		j;
	bool	no_nl;

	i = 1;
	no_nl = false;
	while (thiscmd->args[i] && !(ft_strncmp(thiscmd->args[i], "-n", 2)))
	{
		j = 1;
		while (thiscmd->args[i][j] == 'n')
			j++;
		if (thiscmd->args[i++][j] == '\0')
			no_nl = true;
		else if (thiscmd->args[--i][j] != '\0')
			break ;
	}
	while (thiscmd->args[i++])
	{
		ft_putstr_fd(thiscmd->args[i - 1], STDOUT_FILENO);
		if (thiscmd->args[i])
			write(1, " ", 1);
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
		ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	how_exit(NULL, 1, EXIT_SUCCESS, shell);
}
