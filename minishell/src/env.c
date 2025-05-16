#include "../includes/minishell.h"

char	*remove_outer_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"' && str[len - 1] == '"')))
	{
		char *clean = ft_substr(str, 1, len - 2);
		free(str);
		return clean;
	}
	return str;
}


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

char *expand_env_variable(const char *arg, char **envp)
{
	int		i = 0;
	int		in_single = 0;
	int		in_double = 0;
	char	*result = ft_strdup("");
	char	*var, *val, *tmp;
	char	outer_quote = 0;

	// Detect outermost quote type
	if (arg[0] == '\'' || arg[0] == '"') {
		outer_quote = arg[0];
	}

	while (arg[i])
	{
		// Toggle quote states, but preserve the outermost quote context
		if (arg[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			tmp = ft_strjoin_free(result, "'");
			result = tmp;
			i++;
			continue;
		}
		else if (arg[i] == '"' && !in_single)
		{
			in_double = !in_double;
			tmp = ft_strjoin_free(result, "\"");
			result = tmp;
			i++;
			continue;
		}
		else if (arg[i] == '$' && !in_single && arg[i + 1])
		{
			// Don't expand variables inside single quotes
			if (outer_quote == '\'' && in_single)
			{
				char buf[2] = { arg[i], '\0' };
				tmp = ft_strjoin_free(result, buf);
				result = tmp;
				i++;
				continue;
			}
			
			if (arg[i + 1] == '?')
			{
				val = ft_itoa(g_signal_status);
				tmp = ft_strjoin_free(result, val);
				free(val);
				result = tmp;
				i += 2; // Skip "$?"
				continue;
			}
			int start = ++i;
			while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
				i++;
			var = ft_substr(arg, start, i - start);
			val = find_env_value(var, envp);
			free(var);
			tmp = ft_strjoin_free(result, val);
			free(val);
			result = tmp;
		}
		else
		{
			char buf[2] = { arg[i], '\0' };
			tmp = ft_strjoin_free(result, buf);
			result = tmp;
			i++;
		}
	}
	// Remove only the outermost quotes after expansion
	return (remove_outer_quotes(result));
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
