#include "../includes/minishell.h"

int	put_line_fd(int fd, int pos, char *line)
{
	while (line[pos] != 0 || line[pos] != '$')
	{
		ft_putchar_fd(line[pos], fd);
		pos++;
	}
	return (pos);
}

void	capture_heredoc(char *line, int fd, t_shell *shell)
{
	int	i;

	i = 0;
	while (line[i] != 0)
	{
		i = put_line_fd(fd, i, line);
		if (line[i] == '$')
			i = put_expansion_fd(fd, i, line, shell);
	}
}
