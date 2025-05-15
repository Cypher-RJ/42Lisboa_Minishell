#include "../includes/minishell.h"

int	redirect_input(char *file, t_shell *shell, bool has_fork)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (how_exit("Failed to open output file", \
			has_fork, EXIT_FAILURE, shell));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (how_exit("Failed to dup2 input file fd", \
			has_fork, EXIT_FAILURE, shell));
	}
	close(fd);
	return (EXIT_SUCCESS);
}

int	redirect_heredoc(int hd_fd, t_shell *shell, bool has_fork)
{
	if (hd_fd == -1)
		return (how_exit("Heredoc fd does not exist", \
			has_fork, EXIT_FAILURE, shell));
	if (dup2(hd_fd, STDIN_FILENO) == -1)
	{
		close(hd_fd);
		return (how_exit("Failed to dup2 input file fd", \
			has_fork, EXIT_FAILURE, shell));
	}
	close(hd_fd);
	return (EXIT_SUCCESS);
}

int	redirect_output(char *file, int appd, t_shell *shell, bool has_fork)
{
	int	fd;

	if (appd == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (how_exit("Failed to open output file", \
			has_fork, EXIT_FAILURE, shell));
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (how_exit("Failed to dup2 output file fd", \
			has_fork, EXIT_FAILURE, shell));
	}
	close(fd);
	return (EXIT_SUCCESS);
}

int	redirector(t_redirect *redir, t_shell *shell, bool has_fork)
{
	t_redirect	*temp;
	int			res;

	temp = redir;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->direction, "<<", 2) == 0)
			res = redirect_heredoc(temp->hf_fd, shell, has_fork);
		else if (ft_strncmp(temp->direction, "<", 1) == 0)
			res = redirect_input(temp->passorfile, shell, has_fork);
		else if (ft_strncmp(temp->direction, ">>", 2) == 0)
			res = redirect_output(temp->passorfile, 1, shell, has_fork);
		else if (ft_strncmp(temp->direction, ">", 1) == 0)
			res = redirect_output(temp->passorfile, 0, shell, has_fork);
		if (res == 0)
			temp = temp->next;
		else
			return (res);
	}
	return (EXIT_SUCCESS);
}
