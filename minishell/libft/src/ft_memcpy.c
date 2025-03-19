/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:18:49 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 15:37:05 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*memdest;
	unsigned char	*memsrc;
	size_t			i;

	memsrc = (unsigned char *)src;
	memdest = (unsigned char *)dest;
	i = 0;
	if (!src && !dest)
		return (NULL);
	while (i < n)
	{
		memdest[i] = memsrc[i];
		i++;
	}
	return (dest);
}
