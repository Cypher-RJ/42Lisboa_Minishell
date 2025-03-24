#include "minishell.h"
#include <sys/stat.h>

// Checks if a file exists and is executable
int	is_executable(char *filepath)
{
	struct stat	buffer;

	return (access(filepath, X_OK) == 0 && stat(filepath, &buffer) == 0);
}

// Gets the PATH variable from `envp`
char	*get_env_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_strdup(envp[i] + 5));
		i++;
	}
	return (NULL);
}

// Searches for the full path of a command
char	*search_path(char *cmd, char **paths)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (is_executable(full_path))
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

// Finds the full path of a command
char	*get_path(char *cmd, char **envp)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	int		i;
	char	*temp;

	full_path = NULL;
	i = 0;
	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_path(envp);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	free(path_var);
	while (paths[i] && !full_path)
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) != 0)
		{
			free(full_path);
			full_path = NULL;
		}
		i++;
	}
	ft_free_split(paths);
	return (full_path);
}
