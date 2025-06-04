#include "../includes/minishell.h"

void	handle_notvalue_dollar(t_exp *exp, char *tmp)
{
	tmp = ft_strjoin_free(exp->result, "$");
	exp->result = tmp;
	exp->i++;
}

void	handle_lastexit_dollar(t_exp *exp, char *tmp)
{
	exp->val = ft_itoa(exp->last_exit);
	tmp = ft_strjoin_free(exp->result, exp->val);
	free(exp->val);
	exp->result = tmp;
	exp->i += 2;
}

void	handle_storing_dollar(t_exp *exp, char *tmp)
{
	exp->var = ft_substr(exp->arg, exp->start, exp->i - exp->start);
	exp->val = find_env_value(exp->var, exp->envp);
	free(exp->var);
	tmp = ft_strjoin_free(exp->result, exp->val);
	free(exp->val);
	exp->result = tmp;
}

void	handle_dollar(t_exp *exp)
{
	char	*tmp;

	tmp = NULL;
	if (!exp->arg[exp->i + 1] || (!ft_isalnum(exp->arg[exp->i + 1])
			&& exp->arg[exp->i + 1] != '_' && exp->arg[exp->i + 1] != '?'))
	{
		handle_notvalue_dollar(exp, tmp);
		return ;
	}
	if (exp->arg[exp->i + 1] == '?')
	{
		handle_lastexit_dollar(exp, tmp);
		return ;
	}
	exp->i++;
	exp->start = exp->i;
	while (exp->arg[exp->i] && (ft_isalnum(exp->arg[exp->i])
			|| exp->arg[exp->i] == '_'))
		exp->i++;
	handle_storing_dollar(exp, tmp);
}
