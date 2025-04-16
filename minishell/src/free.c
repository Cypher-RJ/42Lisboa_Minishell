#include "../includes/minishell.h"

void	free_command_list(t_command *cmds)
{
	t_command *tmp;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
			ft_free_split(cmds->args);
		if (cmds->path)
			free(cmds->path);
		free(cmds);
		cmds = tmp;
	}
}

void	free_all(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->envp)
		ft_free_split(shell->envp);
	rl_clear_history();
}