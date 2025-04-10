#include "minishell.h"

char	**copy_shlvl(t_shell *shell)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (shell->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = ft_strdup(shell->envp[i]);
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

void	increment_shlvl(t_shell *shell)
{
	int		i;
	int		shlvl;
	char	*new_shlvl;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(shell->envp[i] + 6) + 1;
			new_shlvl = ft_strjoin("SHLVL=", ft_itoa(shlvl));
			free(shell->envp[i]);
			shell->envp[i] = new_shlvl;
			return ;
		}
		i++;
	}
	shell->envp[i] = ft_strdup("SHLVL=1");
	shell->envp[i + 1] = NULL;
}

int	main(int ac, char **av, t_shell *shell)
{
	char	**my_envp;

	(void)ac;
	(void)av;
	my_envp = copy_shlvl(shell->envp);
	increment_shlvl(my_envp);
	prompt_loop(my_envp);
	ft_free_split(my_envp);
	return (0);
}
