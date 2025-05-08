#include "../includes/minishell.h"

int	is_var_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isalnum(str[i]))
		i++;
	if ((str[i] != '=' && str[i] != '\0') || !ft_isalpha(str[0]))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	get_name_value(char *str, int n_end, int v_end)
{
	char	*name;
	char	*value;
	int		i;

	i = 0;
	name = ft_calloc(n_end + 1, sizeof(char));
	if (!name)
		return (ft_putendl_fd("Export: Failed to calloc var name", \
			STDERR_FILENO), EXIT_FAILURE);
	ft_strlcpy(name, str, n_end +1);
}

int	export_var(char *str, t_shell *shell, bool has_fork)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i++] == '\0')
		return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
	j = i;
	while (str[j])
		j++;
	if (get_name_value(str, i, j))
		return (how_exit(NULL, has_fork, EXIT_FAILURE, shell));
	return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
}
