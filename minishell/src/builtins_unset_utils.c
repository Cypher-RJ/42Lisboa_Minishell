#include "../includes/minishell.h"

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
			break;
		j++;
	}
	if (!shell->envp[j])
		return (EXIT_SUCCESS);
	return (unset_env(shell, j));
}
