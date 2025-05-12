/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:40:49 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/05/12 11:58:49 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(char *instr, char *comparing)
{
	int	i;

	i = 0;
	while (instr[i] == comparing[i] && instr[i])
		i++;
	return (instr[i] - comparing[i]);
}
