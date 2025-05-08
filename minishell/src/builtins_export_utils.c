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

int	export_var(char *str, t_shell *shell, bool has_fork)
{
	if (str && shell && has_fork)
		return (1);
	return (0);
}