#include "../includes/minishell.h"

void	free_redir_list(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->direction);
		free(redir->passorfile);
		if (redir->hf_fd != -1)
			close(redir->hf_fd);
		free(redir);
		redir = tmp;
	}
}

void	free_command_list(t_command *cmds)
{
	t_command	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
			ft_free_split(cmds->args);
		if (cmds->path)
			free(cmds->path);
		if (cmds->redir)
			free_redir_list(cmds->redir);
		free(cmds);
		cmds = tmp;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->envp)
		ft_free_split(shell->envp);
	free(shell);
	rl_clear_history();
}

void	free_total(t_shell *shell)
{
	if (shell->cmds)
		free_command_list(shell->cmds);
	if (shell)
		free_shell(shell);
}
