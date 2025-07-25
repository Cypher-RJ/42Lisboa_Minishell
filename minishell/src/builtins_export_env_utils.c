/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_env_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darkless12 <darkless12@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:18:31 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/07/25 19:11:49 by darkless12       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**copy_env(char **envp)
{
	char	**dest;
	int		i;

	i = 0;
	dest = NULL;
	while (envp[i])
		i++;
	dest = ft_calloc(i + 1, sizeof(char *));
	if (!dest)
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		dest[i] = envp[i];
		i++;
	}
	return (dest);
}

void	sort_envtmp(char **envtmp)
{
	int		i;
	int		j;
	char	*swap;

	i = 0;
	j = 0;
	swap = NULL;
	while (envtmp[i])
	{
		j = i + 1;
		while (envtmp[j])
		{
			if (ft_strcmp(envtmp[i], envtmp[j]) > 0)
			{
				swap = envtmp[i];
				envtmp[i] = envtmp[j];
				envtmp[j] = swap;
			}
			j++;
		}
		i++;
	}
}

int	put_envtmp(char *envtmp)
{
	int		i;
	int		eq;
	char	*line;

	i = 0;
	eq = 0;
	while (envtmp[i] != 0 && envtmp[i] != '=')
		i++;
	if (envtmp[i] == '=')
	{
		i++;
		eq = 1;
	}
	line = calloc(i + 1, sizeof(char));
	if (!line)
		return (1);
	ft_strlcat(line, envtmp, i + 1);
	ft_printf("declare -x %s", line);
	if (envtmp[i] != '\0')
		ft_printf("\"%s\"", &envtmp[i]);
	if (envtmp[i] == '\0' && eq)
		ft_printf("\"\"");
	ft_printf("\n");
	free(line);
	return (0);
}

int	export_putenv(t_shell *shell, bool has_fork)
{
	char	**envtmp;
	int		i;
	int		res;

	i = 0;
	res = 0;
	envtmp = copy_env(shell->envp);
	if (!envtmp)
		return (how_exit("Failed malloc of envtmp for sort", \
has_fork, EXIT_FAILURE, shell));
	sort_envtmp(envtmp);
	while (envtmp[i])
	{
		res = put_envtmp(envtmp[i]);
		if (res)
			return (how_exit("Failed to build export line", \
has_fork, EXIT_FAILURE, shell));
		i++;
	}
	free(envtmp);
	return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
}
