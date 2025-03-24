#include "minishell.h"

char	*expand_env_variable(char *arg, char **envp)
{
	int		i;
	char	*var_name;

	if (arg[0] != '$')
		return (ft_strdup(arg));
	i = 0;
	var_name = arg + 1;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, ft_strlen(var_name)) == 0
			&& envp[i][ft_strlen(var_name)] == '=')
			return (ft_strdup(envp[i] + ft_strlen(var_name) + 1));
		i++;
	}
	return (ft_strdup(""));
}

int	detect_redirections(char **args, int *fd_in, int *fd_out)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0)
		{
			*fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (*fd_out < 0)
			{
				perror("open");
				return (1);
			}
			args[i] = NULL;
			return (1);
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			*fd_in = open(args[i + 1], O_RDONLY);
			if (*fd_in < 0)
			{
				perror("open");
				return (1);
			}
			args[i] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
