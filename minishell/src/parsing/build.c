#include "../../includes/minishell.h"

t_command	*build_command_list(char **split_cmds, t_shell *shell)
{
	t_command	*head;
	t_command	*current;
	char		**args;
	int			i;
	int			j;
	char		*expanded;
	t_command	*new_node;

	head = NULL;
	current = NULL;
	i = 0;
	while (split_cmds[i])
	{
		args = ft_split_quotes(split_cmds[i]);
		if (!args)
		{
			free_commands(head);
			return (NULL);
		}
		j = 0;
		while (args[j])
		{
			if (ft_strlen(args[j]) == 0)
			{
				free(args[j]);
				int k = j;
				while (args[k])
				{
					args[k] = args[k + 1];
					k++;
				}
				continue;
			}
			expanded = expand_env_variable(args[j], shell->envp);
			if (!expanded)
			{
				ft_free_split(args);
				free_commands(head);
				return (NULL);
			}
			args[j] = expanded;
			j++;
		}
		new_node = ft_calloc(1, sizeof(t_command));
		if (!new_node)
		{
			ft_free_split(args);
			free_commands(head);
			return (NULL);
		}
		new_node->args = args;
		new_node->next = NULL;
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	return (head);
}
