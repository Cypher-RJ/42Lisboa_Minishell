/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:27:35 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/06 09:27:36 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
