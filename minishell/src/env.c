#include "../includes/minishell.h"

/* static char	*get_var_name(char *arg)
{
	if (arg[0] != '$' || arg[1] == '\0')
		return (NULL);
	return (arg + 1);
} */

static char	*find_env_value(char *var_name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var_name, len) && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*expand_env_variable(char *arg, char **envp)
{
	int		i = 0;
	char	*result = ft_strdup("");
	char	*tmp;

	while (arg[i])
	{
		if (arg[i] == '\'') // aspas simples -> literal até próxima
		{
			int	start = ++i;
			while (arg[i] && arg[i] != '\'')
				i++;
			tmp = ft_substr(arg, start, i - start);
			result = ft_strjoin_free(result, tmp);
			if (arg[i] == '\'')
				i++;
		}
		else if (arg[i] == '"') // aspas duplas -> permite expansão
		{
			int	start = ++i;
			char	*inside = ft_strdup("");
			while (arg[i] && arg[i] != '"')
			{
				if (arg[i] == '$' && arg[i + 1])
				{
					int	var_start = ++i;
					while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
						i++;
					char *var = ft_substr(arg, var_start, i - var_start);
					char *val = find_env_value(var, envp);
					free(var);
					inside = ft_strjoin_free(inside, val);
					free(val);
				}
				else
				{
					char buf[2] = { arg[i++], '\0' };
					inside = ft_strjoin_free(inside, buf);
				}
			}
			if (arg[i] == '"')
				i++;
			result = ft_strjoin_free(result, inside);
		}
		else if (arg[i] == '$' && arg[i + 1]) // fora de aspas
		{
			int start = ++i;
			while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
				i++;
			char *var = ft_substr(arg, start, i - start);
			char *val = find_env_value(var, envp);
			free(var);
			result = ft_strjoin_free(result, val);
			free(val);
		}
		else
		{
			char buf[2] = { arg[i++], '\0' };
			result = ft_strjoin_free(result, buf);
		}
	}
	free(arg);
	return (result);
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
	int	i;

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
