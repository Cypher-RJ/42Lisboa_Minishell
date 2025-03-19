/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:34:50 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 14:33:54 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_printf.h"

int	ft_printptr(void *ptr, int c)
{
	int				i;
	char			*digits;
	unsigned long	n;

	i = 0;
	n = (unsigned long)ptr;
	if (n == 0)
		return (write(1, "(nil)", 5));
	if (c == 0)
	{
		i += write(1, "0x", 2);
		c = 1;
	}
	digits = "0123456789abcdef";
	if (n < 16)
		i += ft_printchar(digits[n]);
	else
	{
		i += ft_printnbrhex((n / 16), 16);
		i += ft_printchar(digits[n % 16]);
	}
	return (i);
}
