#include "../includes/minishell.h"

void	redirect_input(t_redirect *thiscmd)
{
	int	fd;

	fd = open(thiscmd->passorfile, O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Failed to dup2 file fd");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

int	heredoc_readline(char *password, int fd[])
{
	char *line;

	line = readline("> ");
	if (!line) // isto precisa de um signal handler. Se for ctr-d faz return, outros sinais ou erro faz exit
		return (-1);
	if (strcmp(line, password) == 0)
	{
		free(line);
		return (-1);
	}
	write(fd[1], line, strlen(line));
	write(fd[1], "\n", 1);
	free(line);
	return (0);
}

void	redirect_heredoc(t_redirect *thiscmd)
{
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("Failed to open HEREDOC pipe");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		if (heredoc_readline(thiscmd->passorfile, fd) == -1)
			break;
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("Failed to dup2 HEREDOC fd[0] to STDIN");
		close(fd[0]);
		exit(EXIT_FAILURE);
	}
	close(fd[0]);
}

void	redirect_output(t_redirect *thiscmd, int append)
{
	int	fd;

	if (append == 0)
		fd = open(thiscmd->passorfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(thiscmd->passorfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Failed to dup2 file fd");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	redirector(t_redirect *redir)
{
	t_redirect	*temp;

	temp = redir;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->direction, "<<", 2) == 0)
			redirect_heredoc(temp);
		else if (ft_strncmp(temp->direction, "<", 1) == 0)
			redirect_input(temp);
		else if (ft_strncmp(temp->direction, ">>", 2) == 0)
			redirect_output(temp, 1);
		else if (ft_strncmp(temp->direction, ">", 1) == 0)
			redirect_output(temp, 0);
		temp = temp->next;
	}
}
