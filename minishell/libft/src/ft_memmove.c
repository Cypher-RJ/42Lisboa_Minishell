/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:45:10 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 15:37:12 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*memdest;
	unsigned char	*memsrc;
	size_t			i;

	memdest = (unsigned char *)dest;
	memsrc = (unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (memsrc < memdest)
	{
		i = n;
		while (i-- > 0)
			memdest[i] = memsrc[i];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			memdest[i] = memsrc[i];
			i++;
		}
	}
	return (dest);
}
