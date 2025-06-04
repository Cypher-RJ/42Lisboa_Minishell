#include "../includes/minishell.h"

char	*remove_outer_quotes(char *str)
{
	int		i;
	int		j;
	char	*res;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (str);
	process_chars(str, res, &i, &j);
	free(str);
	return (res);
}

char	*find_env_value(const char *var_name, char **envp)
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
	t_exp	exp;
	char	*tmp;

	if (!arg)
		return (NULL);
	init_exp(&exp, arg, envp, last_exit);
	while (exp.arg[exp.i])
	{
		exp.buf[0] = exp.arg[exp.i];
		if (exp.buf[0] == '\'' && !exp.in_double)
			handle_sord_quotes(0, &exp);
		else if (exp.buf[0] == '"' && !exp.in_single)
			handle_sord_quotes(1, &exp);
		else if (exp.buf[0] == '$' && !exp.in_single)
			handle_dollar(&exp);
		else
		{
			tmp = ft_strjoin_free(exp.result, exp.buf);
			exp.result = tmp;
			exp.i++;
		}
	}
	return (exp.result);
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
