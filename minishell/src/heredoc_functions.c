#include "../includes/minishell.h"

void	heredoc_readline(char *word, int fd[])
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
			exit(-1);
		}
		if (strcmp(line, word) == 0)
		{
			close(fd[1]);
			free(line);
			exit(-1);
		}
		write(fd[1], line, strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	exit(0);
}

int	redirect_heredoc(t_redirect *redir, t_shell *shell)
{
	int		fd[2];
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
		heredoc_readline(redir->passorfile, fd);
	restore_signals();
	close(fd[1]);
	redir->hf_fd = fd[0];
	waitpid(pid, NULL, 0);
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
				res = redirect_heredoc(thisredir, shell);
			if (res)
				return (res);
			thisredir = thisredir->next;
		}
		thiscmd = thiscmd->next;
	}
	return (res);
}
