/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:09:30 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 14:28:10 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdarg.h>
# include <limits.h>
# include "libft.h"

int	ft_printchar(char c);
int	ft_printstr(char *str);
int	ft_printnbr(long nbr, int base);
int	ft_printnbrhex(unsigned long nbr, unsigned long base);
int	ft_printptr(void *ptr, int c);
int	ft_printf(const char *str, ...);

#endif