#include "../includes/minishell.h"

int	increase_envp(t_shell *shell)
{
	char	**envtmp;
	int		i;

	envtmp = NULL;
	i = 0;
	while (shell->envp[i])
		i++;
	envtmp = ft_calloc(i + 2, sizeof(char *));
	if (!envtmp)
		return (write(2, "Failed to malloc new envp", 26), EXIT_FAILURE);
	i = 0;
	while (shell->envp[i] != NULL)
	{
		envtmp[i] = shell->envp[i];
		i++;
	}
	free(shell->envp);
	shell->envp = envtmp;
	return (EXIT_SUCCESS);
}

int	find_tgt_env(char **envp, char *tgt)
{
	size_t	l;
	int		i;

	l = ft_strlen(tgt);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], tgt, l) && (envp[i][l] == '\0' \
			|| envp[i][l] == '='))
			return (i);
		i++;
	}
	return (i);
}

int	rep_add_envp(char *trgt, char *str, t_shell *shell)
{
	char	*tmp;
	int		i;
	int		res;

	tmp = NULL;
	res = 0;
	i = find_tgt_env(shell->envp, trgt);
	if (shell->envp[i] == NULL)
		res = increase_envp(shell);
	if (res)
		return (res);
	if (shell->envp[i] && str == NULL)
		return (res);
	tmp = build_env_str(trgt, str);
	if (!tmp)
		return (write(2, "Failed to add var to envp", 26), EXIT_FAILURE);
	if (shell->envp[i])
		free(shell->envp[i]);
	shell->envp[i] = tmp;
	return (res);
}

int	adjust_envp(char *oldpwd, char *newpwd, bool has_fork, t_shell *shell)
{
	char *temp;

	temp = ft_strjoin("=", oldpwd);
	free(oldpwd);
	oldpwd = temp;
	temp = ft_strjoin("=", newpwd);
	free(newpwd);
	newpwd = temp;
	if (!oldpwd || !newpwd)
		return (how_exit("Failed to create env values for cd", has_fork, EXIT_FAILURE, shell));
	if (rep_add_envp("OLDPWD", oldpwd, shell) \
		|| rep_add_envp("PWD", newpwd, shell))
	{
		free(oldpwd);
		free(newpwd);
		return (how_exit(NULL, has_fork, EXIT_FAILURE, shell));
	}
	free(oldpwd);
	free(newpwd);
	return (EXIT_SUCCESS);
}

// ! cd dentro de um file que se apaga parte
int	builtin_cd_exec(char *strdir, t_shell *shell, bool has_fork)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = get_pwd(shell);
	if (!old_pwd)
		return (how_exit("Failed to allocate to OLDPWD", has_fork, \
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
	if (adjust_envp(old_pwd, new_pwd, has_fork, shell))
		return (EXIT_FAILURE);
	return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
}
