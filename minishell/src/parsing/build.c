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
			if (!is_single_quoted(args[j]))
			{
				char *temp = args[j];
				expanded = expand_env_variable(args[j], shell->envp);
				if (!expanded)
				{
					ft_free_split(args);
					free_commands(head);
					return (NULL);
				}
				args[j] = expanded;
				if (temp != expanded)
					free(temp);
			}
			args[j] = remove_outer_quotes(args[j]);
			if (ft_strlen(args[j]) == 0)
			{
				free(args[j]);
				k = j;
				while (args[k])
				{
					args[k] = args[k + 1];
					k++;
				}
				continue;
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
	t_redirect	*redirect;
	t_redirect	*redir_head;
	t_redirect	*redir_tail;
	int			i;
	t_redirect *temp;
	int			j;

	if (!cmds)
		return (NULL);
	redir_head = NULL;
	redir_tail = NULL;
	i = 0;
	while (cmds->args && cmds->args[i])
	{
		if (cmds->args[i] && (ft_strcmp(cmds->args[i], "<") == 0
				|| ft_strcmp(cmds->args[i], ">") == 0
				|| ft_strcmp(cmds->args[i], "<<") == 0
				|| ft_strcmp(cmds->args[i], ">>") == 0))
		{
			if (!cmds->args[i + 1])
			{
				while (redir_head)
				{
					temp = redir_head;
					redir_head = redir_head->next;
					free(temp->direction);
					free(temp->passorfile);
					free(temp);
				}
				return (NULL);
			}
			redirect = malloc(sizeof(t_redirect));
			if (!redirect)
			{
				while (redir_head)
				{
					temp = redir_head;
					redir_head = redir_head->next;
					free(temp->direction);
					free(temp->passorfile);
					free(temp);
				}
				return (NULL);
			}
			redirect->direction = ft_strdup(cmds->args[i]);
			redirect->passorfile = ft_strdup(cmds->args[i + 1]);
			redirect->hf_fd = -1; //precisei inicializar isto, sorry
			if (!redirect->direction || !redirect->passorfile)
			{
				free(redirect->direction);
				free(redirect->passorfile);
				free(redirect);
				while (redir_head)
				{
					temp = redir_head;
					redir_head = redir_head->next;
					free(temp->direction);
					free(temp->passorfile);
					free(temp);
				}
				return (NULL);
			}
			redirect->next = NULL;
			if (!redir_head)
				redir_head = redirect;
			else
				redir_tail->next = redirect;
			redir_tail = redirect;
			free(cmds->args[i]);
			free(cmds->args[i + 1]);
			j = i;
			while (cmds->args[j + 2])
			{
				cmds->args[j] = cmds->args[j + 2];
				j++;
			}
			cmds->args[j] = NULL;
			cmds->args[j + 1] = NULL;
			continue ;
		}
		i++;
	}
	cmds->redir = redir_head;
	return (cmds);
}
