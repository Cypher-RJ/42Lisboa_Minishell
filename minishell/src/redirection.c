#include "minishell.h"

void	redirect_output(char *filename, int append)
{
	int	fd;

	if (append)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	redirect_input(t_redirect *temp)
{
	int	fd;

	fd = open(temp->passorfile, O_RDONLY);
	if (fd < 0)
		return (perror("open"), 1); //isto precisa de ser gerido melhor. muitos frees e etc
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	redirector(t_redirect *redir, int **fd)
{
	t_redirect	*temp;

	temp = redir;
	while (temp->next != NULL)
	{
		if (ft_strncmp(temp->direction, "<", 1) == 0)
		 	redirect_input(temp);
		else if (ft_strncmp(temp->direction, "<<", 2) == 0)
			redirect_heredoc(temp);
		else if (ft_strncmp(temp->direction, ">", 1) == 0)
			redirect_output(temp);
		else if (ft_strncmp(temp->direction, ">>", 1) == 0)
			redirect_append(temp);
		temp = temp->next;
	}
}