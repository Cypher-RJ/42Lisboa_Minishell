#include "../includes/minishell.h"

int	unset_env(t_shell *shell, int pos)
{
	char	**envtmp;
	int		i;
	int		j;

	envtmp = NULL;
	i = 0;
	j = 0;
	while (shell->envp[i])
		i++;
	if (i > 0)
		envtmp = ft_calloc(i, sizeof(char *));
	if (!envtmp)
		return (write(2, "Failed to malloc new envp", 26), EXIT_FAILURE);
	i = 0;
	while (shell->envp[j] != NULL)
	{
		if (j == pos)
			j++;
		if (shell->envp[j] != NULL)
			envtmp[i++] = shell->envp[j++];
	}
	free(shell->envp[pos]);
	free(shell->envp);
	shell->envp = envtmp;
	return (EXIT_SUCCESS);
}

int	unset_var(char *str, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && ft_isalnum(str[i]))
		i++;
	if (str[i] != '\0' || !ft_isalpha(str[0]))
		return (EXIT_SUCCESS);
	while (shell->envp[j])
	{
		if (!ft_strncmp(shell->envp[j], str, i) && \
			(shell->envp[j][i] == '=' || shell->envp[j][i] == '\0'))
			break ;
		j++;
	}
	if (shell->envp[j] == NULL)
		return (EXIT_SUCCESS);
	return (unset_env(shell, j));
}
