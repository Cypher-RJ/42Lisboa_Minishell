#include "minishell.h"

char **copy_shlvl(char **envp)
{
	int i;
	char **new_envp;

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

void increment_shlvl(char **envp)
{
	int i;
	int shlvl;
	char *new_shlvl;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(envp[i] + 6) + 1;
			new_shlvl = ft_strjoin("SHLVL=", ft_itoa(shlvl));
			free(envp[i]);
			envp[i] = new_shlvl;
			return ;
		}
		i++;
	}
	envp[i] = ft_strdup("SHLVL=1");
	envp[i + 1] = NULL;
}

void	prompt_loop(char **envp)
{
	char	*input;
	char	**args;

	while (1)
	{
		setup_signals();
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		add_history(input);
		args = split_command(input, envp);
		if (args[0])
		{
			if (is_builtin(args[0]))
				execute_builtin(args, envp);
			else
				execute_command(args, envp);
		}
		ft_free_split(args);
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	char **my_envp;

	(void)ac;
	(void)av;
	my_envp = copy_shlvl(envp);
	increment_shlvl(my_envp);
	prompt_loop(my_envp);
	ft_free_split(my_envp);
	return (0);
}
