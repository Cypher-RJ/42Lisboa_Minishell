/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:27:20 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:27:21 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
