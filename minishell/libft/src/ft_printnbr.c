/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:16:23 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 14:33:34 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_printf.h"

int	ft_printnbr(long nbr, int base)
{
	int		i;
	char	*digits;

	digits = "0123456789ABCDEF";
	if (nbr < 0 && base == 10)
	{
		write(1, "-", 1);
		return (ft_printnbr(-nbr, base) + 1);
	}
	else if (nbr < base)
		return (ft_printchar(digits[nbr]));
	else
	{
		i = ft_printnbr(nbr / base, base);
		return (i + ft_printnbr(nbr % base, base));
	}
}
