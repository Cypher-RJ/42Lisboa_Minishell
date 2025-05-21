#include "../includes/minishell.h"

char	*remove_outer_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;
	char	in_quote;

	i = 0;
	j = 0;
	in_quote = 0;
	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (str);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && (i == 0 || str[i - 1] != '\\'))
		{
			if (!in_quote)
				in_quote = str[i];
			else if (in_quote == str[i])
			{
				in_quote = 0;
				i++;
				continue ;
			}
			else
				result[j++] = str[i];
		}
		else if (str[i])
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	free(str);
	return (result);
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

char	*expand_env_variable(const char *arg, char **envp, int last_exit)
{
	int		i;
	int		in_single;
	int		in_double;
	char	*result;
	int		start;
	char	buf[2];
	char	*var;
	char	*val;
	char	*tmp;

	i = 0;
	in_single = 0;
	in_double = 0;
	result = ft_strdup("");
	while (arg[i])
	{
		buf[0] = arg[i];
		buf[1] = '\0';
		if (arg[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			tmp = ft_strjoin_free(result, "'");
			result = tmp;
			i++;
			continue ;
		}
		else if (arg[i] == '"' && !in_single)
		{
			in_double = !in_double;
			tmp = ft_strjoin_free(result, "\"");
			result = tmp;
			i++;
			continue ;
		}
		else if (arg[i] == '$' && !in_single && arg[i + 1] && arg[i + 1] != ' ')
		{
			if (arg[i + 1] == '?')
			{
				val = ft_itoa(last_exit);
				tmp = ft_strjoin_free(result, val);
				free(val);
				result = tmp;
				i += 2;
				continue ;
			}
			start = ++i;
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
			tmp = ft_strjoin_free(result, buf);
			result = tmp;
			i++;
		}
	}
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
