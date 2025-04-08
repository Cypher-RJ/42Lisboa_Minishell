#include "minishell.h"

void	ft_free_split(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char *joined;
	size_t len1;
	size_t len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, len1 + 1);
	ft_strlcat(joined, s2, len1 + len2 + 1);
	free(s1);
	return (joined);
}

char	*ft_strjoin_chr(char *s, char c)
{
	char	*new;
	size_t	len;

	len = ft_strlen(s);
	new = malloc(len + 2);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len);
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}