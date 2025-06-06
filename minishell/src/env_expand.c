/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:19:01 by ddiogo-f          #+#    #+#             */
/*   Updated: 2025/06/06 09:19:05 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_dollar_next_quote(t_exp *exp)
{
	if (exp->arg[exp->i + 1] == '\'' || exp->arg[exp->i + 1] == '"')
	{
		if (exp->in_single == 0 && exp->in_double == 0)
		{
			exp->i++;
			return (1);
		}
	}
	return (0);
}

static void	process_variable(t_exp *exp)
{
	if (skip_dollar_next_quote(exp) == 0)
		handle_dollar(exp);
}

static void	append_char_to_result(t_exp *exp)
{
	char	*tmp;

	tmp = ft_strjoin_free(exp->result, exp->buf);
	exp->result = tmp;
	exp->i++;
}

static void	process_char(t_exp *exp)
{
	char	c;

	c = exp->arg[exp->i];
	exp->buf[0] = c;
	if (c == '\'' && exp->in_double == 0)
		handle_sord_quotes(0, exp);
	else if (c == '"' && exp->in_single == 0)
		handle_sord_quotes(1, exp);
	else if (c == '$' && exp->in_single == 0)
		process_variable(exp);
	else
		append_char_to_result(exp);
}

char	*expand_env_variable(const char *arg, char **envp, int last_exit)
{
	t_exp	exp;
	char	*ret;

	if (arg == NULL)
		return (NULL);
	init_exp(&exp, arg, envp, last_exit);
	while (exp.arg[exp.i] != '\0')
	{
		process_char(&exp);
	}
	ret = exp.result;
	return (ret);
}
