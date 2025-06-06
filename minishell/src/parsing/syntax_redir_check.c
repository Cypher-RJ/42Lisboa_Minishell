/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_redir_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:26:40 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:26:41 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_redir_quote_state(char c, int *pos, char *input)
{
	if (c == '\'' && !pos[3] && (pos[0] == 0 || input[pos[0] - 1] != '\\'))
		pos[2] = !pos[2];
	else if (c == '"' && !pos[2] && (pos[0] == 0 || input[pos[0] - 1] != '\\'))
		pos[3] = !pos[3];
}

static int	handle_redirection_syntax(char *input, int *pos)
{
	if (input[pos[0] + 1] == '>' || input[pos[0] + 1] == '<')
	{
		if (input[pos[0] + 2] == '>' || input[pos[0] + 2] == '<')
			return (print_syntax_error("error near unexpected token `>'"));
		pos[0]++;
	}
	pos[0]++;
	while (input[pos[0]] == ' ' || input[pos[0]] == '\t')
		pos[0]++;
	if (!input[pos[0]] || input[pos[0]] == '>' || input[pos[0]] == '<'
		|| input[pos[0]] == '|')
		return (print_syntax_error("error near unexpected token `newline'"));
	pos[1] = 0;
	return (0);
}

int	check_syntax_redir(char *input)
{
	int	pos[4];

	pos[0] = 0;
	pos[1] = 1;
	pos[2] = 0;
	pos[3] = 0;
	while (input[pos[0]])
	{
		update_redir_quote_state(input[pos[0]], pos, input);
		if (!pos[2] && !pos[3])
		{
			if (input[pos[0]] == '>' || input[pos[0]] == '<')
			{
				if (handle_redirection_syntax(input, pos))
					return (1);
				continue ;
			}
			else if (input[pos[0]] != ' ' && input[pos[0]] != '\t')
				pos[1] = 0;
		}
		pos[0]++;
	}
	if (pos[1])
		return (print_syntax_error("error near unexpected token `newline'"));
	return (0);
}
