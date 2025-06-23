/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:19:34 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/17 14:10:51 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	manage_execve(int err, char *path, t_shell *shell, bool has_fork)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (err == EACCES)
		how_exit(": Permission denied", has_fork, 126, shell);
	else if (err == ENOENT)
		how_exit(": No such file or directory", has_fork, 127, shell);
	else if (errno == EISDIR)
		how_exit(": Is a directory", has_fork, 126, shell);
	else if (errno == ENOEXEC)
		how_exit(": Exec format error", has_fork, 126, shell);
	else
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		how_exit(strerror(err), has_fork, 1, shell);
	}
}

void	execute_command(t_command *cmd, t_shell *shell, bool has_fork)
{
	struct stat	st;

	if (cmd->args[0] == NULL)
		how_exit(NULL, has_fork, EXIT_SUCCESS, shell);
	if (cmd->path == NULL || cmd->args[0][0] == 0|| cmd->args[0][0] == ' ')
	{
		ft_putstr_fd("minishell: '", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		how_exit("': command not found", has_fork, 127, shell);
	}
	if (stat(cmd->path, &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->path, STDERR_FILENO);
		how_exit(": Is a directory", has_fork, 126, shell);
	}
	if (execve(cmd->path, cmd->args, shell->envp) == -1)
		manage_execve(errno, cmd->path, shell, has_fork);
}

int	execute_builtin(t_command *cmds, t_shell *shell, bool has_fork)
{
	int	out;

	out = 0;
	if (!has_fork && cmds->redir)
	{
		if (redirector(cmds->redir, shell, 0) != 0)
			return (how_exit(NULL, 0, EXIT_FAILURE, shell));
	}
	if (!ft_strcmp(cmds->args[0], "cd"))
		out = builtin_cd(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "unset"))
		out = builtin_unset(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "exit"))
		out = builtin_exit(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "export"))
		out = builtin_export(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "pwd"))
		builtin_pwd(shell);
	else if (!ft_strcmp(cmds->args[0], "echo"))
		builtin_echo(cmds, shell);
	else if (!ft_strcmp(cmds->args[0], "env"))
		builtin_env(cmds, shell);
	return (out);
}

void	executor(t_shell *shell)
{
	if (find_heredocs(shell))
	{
		free_command_list(shell->cmds);
		return ;
	}
	if (shell->cmds->next == NULL && shell->cmds->args[0] && \
		(is_unique_builtin(shell->cmds->args[0]) == 1))
		execute_builtin(shell->cmds, shell, 0);
	else
		executor_fork(shell);
	free_command_list(shell->cmds);
	shell->cmds = NULL;
}

/*valgrind --suppressions=readline.supp --show-leak-kinds=all 
--leak-check=full --track-origins=yes --track-fds=yes ./minishell*/
