/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:27:50 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:27:51 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_executable(char *filepath)
{
	struct stat	buffer;

	return (access(filepath, X_OK) == 0 && stat(filepath, &buffer) == 0);
}

char	*get_env_path(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
			return (ft_strdup(shell->envp[i] + 5));
		i++;
	}
	return (NULL);
}

char	*search_path(char *cmd, char **paths)
{
	int		i;
	char	*full_path;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		if (is_executable(full_path))
		{
			free(tmp);
			return (full_path);
		}
		free(tmp);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, t_shell *shell)
{
	char	*path_var;
	char	**paths;
	char	*full_path;

	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_path(shell);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	free(path_var);
	full_path = search_path(cmd, paths);
	ft_free_split(paths);
	return (full_path);
}

void	resolve_path(t_command *cmds, t_shell *shell)
{
	t_command	*current;

	current = cmds;
	while (current)
	{
		if (current->args && current->args[0] && !is_builtin(current->args[0]))
		{
			current->path = get_path(current->args[0], shell);
		}
		current = current->next;
	}
}
