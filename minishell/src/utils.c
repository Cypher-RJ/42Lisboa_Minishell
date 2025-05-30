#include "../includes/minishell.h"

void	free_commands(t_command *head)
{
	t_command	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		ft_free_split(temp->args);
		free(temp);
	}
}

void	ft_free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
	{
		free(s1);
		return (NULL);
	}
	ft_strlcpy(joined, s1, len1 + 1);
	ft_strlcat(joined, s2, len1 + len2 + 1);
	free(s1);
	return (joined);
}

char	*ft_strjoin_chr(char *s, char c)
{
	char	*new;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	new = malloc(len + 2);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len);
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}

int	print_syntax_error(char *msg)
{
	g_signal_status = 2;
	ft_putstr_fd("minishell: syntax error", 2);
	ft_putstr_fd(msg, 2);
	return (1);
}
