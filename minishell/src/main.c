/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:27:47 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:27:48 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal_status;

char	**copy_shlvl(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

void	increment_shlvl(char **envp)
{
	int		i;
	int		shlvl;
	char	*new_shlvl;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(envp[i] + 6) + 1;
			tmp = ft_itoa(shlvl);
			new_shlvl = ft_strjoin("SHLVL=", tmp);
			free(tmp);
			free(envp[i]);
			envp[i] = new_shlvl;
			return ;
		}
		i++;
	}
	envp[i] = ft_strdup("SHLVL=1");
	envp[i + 1] = NULL;
}

void	disable_echoctl(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int	main(int ac, char **av, char **envp)
{
	char	**my_envp;
	t_shell	*shell;

	(void)ac;
	(void)av;
	disable_echoctl();
	shell = malloc(sizeof(t_shell));
	my_envp = copy_shlvl(envp);
	increment_shlvl(my_envp);
	shell->envp = my_envp;
	shell->exit_status = 0;
	prompt_loop(shell);
	return (0);
}
