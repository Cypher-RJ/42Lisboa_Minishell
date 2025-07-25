/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_pwd_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darkless12 <darkless12@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:27:03 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/07/25 19:11:18 by darkless12       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cd_go_home(t_shell *shell, bool has_fork)
{
	int	i;

	i = 0;
	while (shell->envp[i] && ft_strncmp(shell->envp[i], "HOME=", 5))
		i++;
	if (!shell->envp[i] || shell->envp[i][5] == '\0')
		return (how_exit("minishell: cd: HOME not set", has_fork, \
EXIT_FAILURE, shell));
	else
		return (builtin_cd_exec(&shell->envp[i][5], shell, has_fork));
}

int	cd_go_oldpwd(t_command *thiscmd, t_shell *shell, bool has_fork)
{
	int	i;

	i = 0;
	while (shell->envp[i] && ft_strncmp(shell->envp[i], "OLDPWD=", 7))
		i++;
	if (!shell->envp[i] || shell->envp[i][7] == '\0')
		return (how_exit("minishell: cd: OLDPWD not set", has_fork, \
EXIT_FAILURE, shell));
	free(thiscmd->args[1]);
	thiscmd->args[1] = ft_strdup(&shell->envp[i][7]);
	if (!thiscmd->args[1])
		return (how_exit("minishell: cd: Failed malloc from OLDPWD", \
has_fork, EXIT_FAILURE, shell));
	else
		return (builtin_cd_exec(thiscmd->args[1], shell, has_fork));
}

char	*get_pwd(t_shell *shell)
{
	int		i;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		i = 0;
		while (shell->envp[i] && ft_strncmp(shell->envp[i], "PWD=", 4))
			i++;
		if (!shell->envp[i] || shell->envp[i][4] == '\0')
			return (NULL);
		else
			pwd = ft_strdup(&shell->envp[i][4]);
	}
	return (pwd);
}
