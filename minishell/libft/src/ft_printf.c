/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:08:19 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 14:33:22 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_printf.h"

int	print(char format, va_list ap)
{
	int	i;

	i = 0;
	if (format == 'c')
		i += ft_printchar(va_arg(ap, int));
	else if (format == 's')
		i += ft_printstr(va_arg(ap, char *));
	else if (format == 'p')
		i += ft_printptr(va_arg(ap, void *), 0);
	else if (format == 'd' || format == 'i')
		i += ft_printnbr(va_arg(ap, int), 10);
	else if (format == 'u')
		i += ft_printnbr((long)va_arg(ap, unsigned int), 10);
	else if (format == 'x')
		i += ft_printnbrhex((long)va_arg(ap, unsigned int), 16);
	else if (format == 'X')
		i += ft_printnbr((long)va_arg(ap, unsigned int), 16);
	else
		i += write (1, &format, 1);
	return (i);
}

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		i;

	i = 0;
	if (!str)
		return (-1);
	va_start(ap, str);
	while (*str)
	{
		if (*str == '%')
			i += print(*(++str), ap);
		else
			i += write(1, str, 1);
		str++;
	}
	va_end(ap);
	return (i);
}
