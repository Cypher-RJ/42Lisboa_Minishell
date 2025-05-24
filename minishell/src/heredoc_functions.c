#include "../includes/minishell.h"

void	eof_heredoc(char *word)
{
	ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `"\
		, STDERR_FILENO);
	ft_putstr_fd(word, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
	//how_exit(NULL, 1, EXIT_SUCCESS, shell);
}

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
			if (g_signal_status)
				how_exit(NULL, 1, 128 + SIGINT, shell);
			else
				how_exit(NULL, 1, EXIT_SUCCESS, shell);
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
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(fd[0]), shell->exit_status = 128 + SIGINT, 1);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (close(fd[0]), 1);
	else
		eof_heredoc(redir->passorfile);
	restore_signals();
	return (redir->hf_fd = fd[0], EXIT_SUCCESS);
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
