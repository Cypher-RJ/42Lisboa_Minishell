#include "../includes/minishell.h"

char	**copy_env(char **envp)
{
	char	**dest;
	int		i;

	i = 0;
	dest = NULL;
	while (envp[i])
		i++;
	dest = ft_calloc(i + 1, sizeof(char *));
	if (!dest)
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		dest[i] = envp[i];
		i++;
	}
	return (dest);
}
void	sort_envtmp(char **envtmp)
{
	int		i;
	int		j;
	char	*swap;

	i = 0;
	j = 0;
	swap = NULL;
	while (envtmp[i])
	{
		j = i + 1;
		while (envtmp[j])
		{
			if (ft_strcmp(envtmp[i], envtmp[j]) > 0)
			{
				swap = envtmp[i];
				envtmp[i] = envtmp[j];
				envtmp[j] = swap;
			}
			j++;
		}
		i++;
	}
}

int export_putenv(t_shell *shell, bool has_fork)
{
	char	**envtmp;
	int		i;

	i = 0;
	envtmp = copy_env(shell->envp);
	if (!envtmp)
		return (how_exit("Failed malloc of envtmp for sort", \
			has_fork, EXIT_FAILURE, shell));
	sort_envtmp(envtmp);
	while (envtmp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(envtmp[i], STDOUT_FILENO);
		i++;
	}
	free(envtmp);
	return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
}
