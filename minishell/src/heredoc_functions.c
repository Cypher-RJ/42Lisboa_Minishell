/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:19:58 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/23 14:20:28 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	eof_heredoc(char *word)
{
	if (g_signal_status == 0 && word)
	{
		ft_putstr_fd("warning: here-document delimited by end-of-file \
			(wanted `", STDERR_FILENO);
		ft_putstr_fd(word, STDERR_FILENO);
		ft_putendl_fd("')", STDERR_FILENO);
	}
}

int	check_child(int status, int fd[2], t_shell *shell, t_redirect *redir)
{
	int	code;

	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		if (code == 130)
			return (close(fd[0]), shell->exit_status = code, 1);
		else if (code == 42)
			eof_heredoc(redir->passorfile);
		else if (code != 0)
			return (close(fd[0]), 1);
	}
	return (EXIT_SUCCESS);
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
				how_exit(NULL, 1, 42, shell);
		}
		if (strcmp(line, word) == 0)
		{
			close(fd[1]);
			free(line);
			how_exit(NULL, 1, EXIT_SUCCESS, shell);
		}
		capture_heredoc(line, fd[1], shell);
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
	if (check_child(status, fd, shell, redir))
		return (EXIT_FAILURE);
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
				return (setup_signals(), res);
			thisredir = thisredir->next;
		}
		thiscmd = thiscmd->next;
	}
	return (setup_signals(), res);
}
