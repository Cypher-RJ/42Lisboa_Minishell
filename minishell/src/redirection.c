#include "minishell.h"

void	redirect_input(t_redirect *temp)
{
	int	fd;

	fd = open(temp->passorfile, O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open file");
		//free tudo
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Failed to dup2 file fd");
		close(fd);
		//free tudo
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	redirect_output(t_redirect *temp, int append)
{
	int	fd;

	if (append == 0)
		fd = open(temp->passorfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(temp->passorfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("Failed to open file");
		//free tudo
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Failed to dup2 file fd");
		close(fd);
		//free tudo
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	redirect_heredoc(t_redirect *temp)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (strcmp(line, temp->passorfile) == 0)
		{
			free(line);
			break;
		}
		write(fd[1], line, strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

void	redirector(t_redirect **redir)
{
	t_redirect	*temp;

	temp = *redir;
	while (temp->next != NULL)
	{
		if (ft_strncmp(temp->direction, "<", 1) == 0)
			redirect_input(temp);
		else if (ft_strncmp(temp->direction, "<<", 2) == 0)
			redirect_heredoc(temp);
		else if (ft_strncmp(temp->direction, ">", 1) == 0)
			redirect_output(temp, 0);
		else if (ft_strncmp(temp->direction, ">>", 2) == 0)
			redirect_output(temp, 1);
		temp = temp->next;
	}
}