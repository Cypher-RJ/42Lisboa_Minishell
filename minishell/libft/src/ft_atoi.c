/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:54:57 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 15:35:26 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_atoi(const char *nptr)
{
	int	num;
	int	i;
	int	isneg;

	isneg = 1;
	i = 0;
	num = 0;
	if (!nptr)
		return (0);
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			isneg = -1;
		else
			isneg = 1;
		i++;
	}
	if (nptr[i] == '+' || nptr[i] == '-')
		return (0);
	while (nptr[i] >= '0' && nptr[i] <= '9')
		num = (num * 10) + (nptr[i++] - '0');
	return (num * isneg);
}
