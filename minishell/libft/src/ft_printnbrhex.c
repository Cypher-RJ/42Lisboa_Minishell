/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbrhex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:59:58 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 14:33:43 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_printf.h"

int	ft_printnbrhex(unsigned long nbr, unsigned long base)
{
	int		i;
	char	*digits;

	digits = "0123456789abcdef";
	if (nbr < base)
		return (ft_printchar(digits[nbr]));
	else
	{
		i = ft_printnbrhex(nbr / base, base);
		return (i + ft_printnbrhex(nbr % base, base));
	}
}
