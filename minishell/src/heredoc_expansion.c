#include "../includes/minishell.h"

int	put_line_hd(int fd, int pos, char *line)
{
	while (line[pos] != 0 && line[pos] != '$')
	{
		ft_putchar_fd(line[pos], fd);
		pos++;
	}
	return (pos);
}

int	put_expansion_hd(int fd, int pos, char *line, t_shell *shell)
{
	int	i;//posicao final da expansao
	int j;

	i = pos;
	j = 0;
	if (!ft_isalpha(line[pos]))
		return (ft_putchar_fd(line[pos - 1], fd), pos);
	while (line[i] && ft_isalnum(line[i]))
		i++;
	while (shell->envp[j])
	{
		if (!ft_strncmp(&line[pos], shell->envp[j], i - pos) && \
			shell->envp[j][i - pos] == '=')
			return (ft_putstr_fd(&shell->envp[j][i - pos + 1], fd), i);
		j++;
	}
	return (pos);
}

void	capture_heredoc(char *line, int fd, t_shell *shell)
{
	int	i;

	i = 0;
	while (line[i] != 0)
	{
		i = put_line_hd(fd, i, line);
		if (line[i] == '$')
			i = put_expansion_hd(fd, i + 1, line, shell);
	}
	ft_putchar_fd('\n', fd);
}
