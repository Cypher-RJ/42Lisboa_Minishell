#include "../includes/minishell.h"

static int	has_unquoted_space(const char *str)
{
	int	in_single = 0, in_double = 0, i;

	in_single = 0, in_double = 0, i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == ' ' && !in_single && !in_double)
			return (1);
		i++;
	}
	return (0);
}

void	check_and_split_expanded_command(t_command *cmd)
{
	char	**split;
	int		i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (has_unquoted_space(cmd->args[0]))
	{
		split = ft_split_quotes(cmd->args[0]);
		if (!split)
			return ;
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		cmd->args = split;
	}
}
