#include "../../includes/minishell.h"

t_command	*build_command_list(char **cmds, t_shell *shell)
{
	t_command	*head;
	t_command	*current;
	char		**args;
	int			i;
	int			j;
	char		*expanded;
	t_command	*new_node;
	int			k;

	if (!cmds || !cmds[0])
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (cmds[i])
	{
		args = ft_split_quotes(cmds[i]);
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
				k = j;
				while (args[k])
				{
					args[k] = args[k + 1];
					k++;
				}
				continue ;
			}
			if (!is_single_quoted(args[j]))
			{
				expanded = expand_env_variable(args[j], shell->envp);
				if (!expanded)
				{
					ft_free_split(args);
					free_commands(head);
					return (NULL);
				}
				args[j] = expanded;
			}
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

t_command	*build_redir(t_command *cmds)
{
    t_redirect *redirect;
    t_redirect *redir_head;
    t_redirect *redir_tail;
    int i;

    redir_head = NULL;
    redir_tail = NULL;
    i = 0;
    while (cmds->args[i])
    {
        if (ft_strcmp(cmds->args[i], "<") == 0 || ft_strcmp(cmds->args[i], ">") == 0 ||
            ft_strcmp(cmds->args[i], "<<") == 0 || ft_strcmp(cmds->args[i], ">>") == 0)
        {
            // Allocate and populate a new t_redirect node
            redirect = malloc(sizeof(t_redirect));
            if (!redirect)
                return (NULL); // Handle allocation failure
            redirect->direction = ft_strdup(cmds->args[i]); // Duplicate the operator
            redirect->passorfile = ft_strdup(cmds->args[i + 1]); // Duplicate the target
            redirect->next = NULL;

            // Link the new node to the redirection list
            if (!redir_head)
                redir_head = redirect;
            else
                redir_tail->next = redirect;
            redir_tail = redirect;

            // Remove redirection elements from args
            free(cmds->args[i]);     // Free the operator
            free(cmds->args[i + 1]); // Free the target
            int j = i;
            while (cmds->args[j + 2])
            {
                cmds->args[j] = cmds->args[j + 2];
                j++;
            }
            cmds->args[j] = NULL; // Null-terminate the array
            continue; // Skip incrementing i to recheck the shifted args
        }
        i++;
    }
    cmds->redir = redir_head; // Assign the redirection list to the command
    return (cmds);
}
