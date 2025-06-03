#include "../includes/minishell.h"

void	handle_input_util(int k, t_command **cmds, char *input, char **strs)
{
	if (k == 0)
	{
		*cmds = NULL;
		free(input);
	}
	else if (k == 1)
	{
		ft_free_split(strs);
		free_command_list(*cmds);
		*cmds = NULL;
	}
}
