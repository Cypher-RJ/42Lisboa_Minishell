#include "../includes/minishell.h"

int	rep_add_arrstr(char *trgt, char *str, t_shell *shell, bool has_fork)
{
	char	*tmp;
	char	**envtmp;
	int		i;

	envtmp = NULL;
	tmp	= NULL;
	i = 0;
	while (shell->envp[i] && ft_strncmp(shell->envp[i], trgt, ft_strlen(trgt)))
		i++;
	if (shell->envp[i] == NULL)
	{
		envtmp = ft_calloc(i + 2, sizeof(char *));
		if (!envtmp)
			return (how_exit("Failed to malloc envtmp", has_fork, EXIT_FAILURE, shell));
		i = 0;
		while (shell->envp[i] != NULL)
		{
			envtmp[i] = shell->envp[i];
			i++;
		}
		ft_free_split(shell->envp);
		shell->envp = envtmp;
	}
}

int builtin_cd_exec(char *strdir, t_shell *shell, bool has_fork)
{
    char *old_pwd;
	char *new_pwd;

    old_pwd = getcwd(NULL, 0);
	if (!old_pwd && shell)
		return (how_exit("getcwd() failed to allocate", has_fork, \
			EXIT_FAILURE, shell));
	if (chdir(strdir) != 0)
	{
		free(old_pwd);
		perror("minishell: cd");
		return (how_exit(NULL, has_fork, EXIT_FAILURE, shell));
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		free(old_pwd);
		return (how_exit("getcwd() failed to allocate", has_fork, \
			EXIT_FAILURE, shell));
	}
    return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
}
