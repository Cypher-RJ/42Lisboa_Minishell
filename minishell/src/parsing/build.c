#include "minishell.h"

char	**ft_split_quotes(char *str, char sep)
{
	char	**res;
	int		count = 0;
	int		i = 0;
	int		start = 0;
	int		in_quote = 0;

	res = malloc(sizeof(char *) * 100); // mock tamanho maximo
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"')
			in_quote = !in_quote;
		else if (str[i] == sep && !in_quote)
		{
			res[count++] = strndup(str + start, i - start);
			while (str[i] == sep)
				i++;
			start = i;
			continue;
		}
		i++;
	}
	if (start < i)
		res[count++] = strndup(str + start, i - start);
	res[count] = NULL;
	return (res);
}

t_command	*build_command_list(char *input, t_shell *shell)
{
	t_command	*head;
	t_command	*curr;
	t_command	*new_node;
	char		**split_by_pipe;
	int			i;

	i = 0;
	head = NULL;
	split_by_pipe = split_cmds(input, shell);
	if (!split_by_pipe)
		return (NULL);
	while (split_by_pipe[i])
	{
		new_node = malloc(sizeof(t_command));
		if (!new_node)
			return (NULL); //! ideal: free lista anterior

		new_node->args = ft_split_quotes(split_by_pipe[i], ' ');
		new_node->path = NULL;
		new_node->redir = NULL;
		new_node->next = NULL;

		if (!head)
			head = new_node;
		else
			curr->next = new_node;
		curr = new_node;
		i++;
	}
	ft_free_split(split_by_pipe);
	return (head);
}
