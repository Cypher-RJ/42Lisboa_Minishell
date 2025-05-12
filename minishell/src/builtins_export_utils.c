#include "../includes/minishell.h"

char	**build_env_str(char *trgt, char *str, char *envp)
{
	int	size;

	size = ft_strlen(trgt);
	if (envp )
}

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
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	make_name_value(char *str, int n_end, int v_end, t_shell *shell)
{
	char	*name;
	char	*value;
	int		res;

	res = 0;
	name = ft_calloc(n_end + 1, sizeof(char));
	if (!name)
		return (ft_putendl_fd("Export: Failed to calloc var name", \
			STDERR_FILENO), EXIT_FAILURE);
	ft_strlcpy(name, str, n_end + 1);
	if (v_end == n_end)
		value = NULL;
	else
		value = ft_calloc(v_end - n_end + 1, sizeof(char));
	if (!value && v_end != n_end)
		return (ft_putendl_fd("Export: Failed to calloc var value", \
			STDERR_FILENO), EXIT_FAILURE);
	else if (value && v_end != n_end)
		ft_strlcpy(value, &str[n_end], v_end - n_end + 1);
	res = rep_add_envp(name, value, shell);
	free(name);
	if (value)
		free(value);
	return (res);
}

int	export_var(char *str, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i] == '\0')
		j = i;
	else
		j = ++i;
	while (str[j])
		j++;
	if (make_name_value(str, i, j, shell))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
