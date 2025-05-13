#include "../includes/minishell.h"

void	child_labor(int prev_fd, t_command *thiscmd, int fd[], t_shell *shell)
{
	restore_signals();
	child_pipes(prev_fd, (thiscmd->next != NULL), fd, shell);
	redirector(thiscmd->redir, shell, 1);
	execute_builtin(thiscmd, shell, 1);
	execute_command(thiscmd, shell);
}

int	daddy_time(int *prev_fd, bool next, int fd[])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (next == 1)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	return (*prev_fd);
}

void	executor_fork(t_shell *shell)
{
	t_command	*thiscmd;
	int			fd[2];
	int			prev_fd;
	pid_t		pid;

	thiscmd = shell->cmds;
	prev_fd = -1;
	while (thiscmd != NULL)
	{
		if (make_pipe(fd, (thiscmd->next != NULL)) == -1)
			break ;
		pid = fork_it();
		if (pid == -1)
			break ;
		if (pid == 0)
			child_labor(prev_fd, thiscmd, fd, shell);
		prev_fd = daddy_time(&prev_fd, (thiscmd->next != NULL), fd);
		thiscmd = thiscmd->next;
	}
	thiscmd = shell->cmds;
	while (thiscmd != NULL)
	{
		wait(NULL);
		thiscmd = thiscmd->next;
	}
}
