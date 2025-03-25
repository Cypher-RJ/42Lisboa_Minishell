#include "minishell.h"

static char	*get_var_name(char *arg)
{
	if (arg[0] != '$' || arg[1] == '\0')
		return (NULL);
	return (arg + 1);
}

static char	*find_env_value(char *var_name, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, ft_strlen(var_name)) == 0
			&& envp[i][ft_strlen(var_name)] == '=')
			return (ft_strdup(envp[i] + ft_strlen(var_name) + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*expand_env_variable(char *arg, char **envp)
{
	char	*var_name;

	var_name = get_var_name(arg);
	if (!var_name)
		return (ft_strdup(arg));
	return (find_env_value(var_name, envp));
}

static int	handle_redirection(char **args, int *fd, int flags, int i)
{
	if (*fd != -1)
		close(*fd);
	*fd = open(args[i + 1], flags, 0644);
	if (*fd < 0)
	{
		perror("open");
		return (1);
	}
	args[i] = NULL;
	args[i + 1] = NULL;
	return (0);
}

int	detect_redirections(char **args, int *fd_in, int *fd_out)
{
	int i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0)
		{
			if (!args[i + 1])
				return (fprintf(stderr, "minishell: syntax error near `>`\n"),
					1);
			if (handle_redirection(args, fd_out, O_WRONLY | O_CREAT | O_TRUNC,
					i))
				return (1);
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			if (!args[i + 1])
				return (fprintf(stderr, "minishell: syntax error near `<`\n"),
					1);
			if (handle_redirection(args, fd_in, O_RDONLY, i))
				return (1);
		}
		i++;
	}
	return (0);
}
