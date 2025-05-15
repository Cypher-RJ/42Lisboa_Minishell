#include "../includes/minishell.h"

void	heredoc_readline(char *word, int fd[], t_shell *shell)
{
	char	*line;

	close(fd[0]);
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd[1]);
			how_exit(NULL, 1, EXIT_FAILURE, shell);
		}
		if (strcmp(line, word) == 0)
		{
			close(fd[1]);
			free(line);
			how_exit(NULL, 1, EXIT_SUCCESS, shell);
		}
		write(fd[1], line, strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	exit(0);
}

int	store_heredoc(t_redirect *redir, t_shell *shell)
{
	int		fd[2];
	int		status;
	pid_t	pid;

	if (pipe(fd) < 0)
		return (perror("Failed to pipe heredoc\n"), 1);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (perror("Failed to fork heredoc\n"), 1);
	}
	if (pid == 0)
		heredoc_readline(redir->passorfile, fd, shell);
	restore_signals();
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (shell->exit_status = 128 + SIGINT, 1);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (1);
	redir->hf_fd = fd[0];
	return (EXIT_SUCCESS);
}

int	find_heredocs(t_shell *shell)
{
	t_command	*thiscmd;
	t_redirect	*thisredir;
	int			res;

	res = 0;
	thiscmd = shell->cmds;
	while (thiscmd)
	{
		thisredir = thiscmd->redir;
		while (thisredir)
		{
			thisredir->hf_fd = -1;
			if (ft_strncmp(thisredir->direction, "<<", 2) == 0)
				res = store_heredoc(thisredir, shell);
			if (res)
				return (res);
			thisredir = thisredir->next;
		}
		thiscmd = thiscmd->next;
	}
	return (res);
}
