#include "../includes/minishell.h"

int	unset_var(char *str, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i++] == '\0')
		return (EXIT_SUCCESS);
	j = i;
	while (str[j])
		j++;
	if (make_name_value(str, i, j, shell))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}