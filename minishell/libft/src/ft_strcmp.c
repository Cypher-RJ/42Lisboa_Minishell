/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:40:49 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/11/11 10:54:03 by rcesar-d         ###   ########.fr       */
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
