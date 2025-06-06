/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:19:20 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/06 09:19:28 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_chars(char *str, char *res, int *i, int *j)
{
	char	in_quote;

	in_quote = 0;
	while (str[*i])
	{
		if ((str[*i] == '\'' || str[*i] == '"')
			&& (*i == 0 || str[*i - 1] != '\\'))
		{
			if (!in_quote)
				in_quote = str[*i];
			else if (in_quote == str[*i])
			{
				in_quote = 0;
				(*i)++;
				continue ;
			}
			else
				res[(*j)++] = str[*i];
		}
		else if (str[*i])
			res[(*j)++] = str[*i];
		(*i)++;
	}
	res[*j] = '\0';
}

void	init_exp(t_exp *exp, const char *arg, char **envp, int last_exit)
{
	exp->arg = arg;
	exp->envp = envp;
	exp->last_exit = last_exit;
	exp->i = 0;
	exp->in_single = 0;
	exp->in_double = 0;
	exp->start = 0;
	exp->var = NULL;
	exp->val = NULL;
	exp->buf[1] = '\0';
	exp->result = ft_strdup("");
	if (!exp->result)
		exit(EXIT_FAILURE);
}

void	handle_sord_quotes(int s_or_d, t_exp *exp)
{
	char	*tmp;

	if (s_or_d == 0)
	{
		exp->in_single = !exp->in_single;
		tmp = ft_strjoin_free(exp->result, "\'");
		exp->result = tmp;
		exp->i++;
	}
	else if (s_or_d == 1)
	{
		exp->in_double = !exp->in_double;
		tmp = ft_strjoin_free(exp->result, "\"");
		exp->result = tmp;
		exp->i++;
	}
}
