/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:15:13 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 15:36:58 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*mems1;
	unsigned char	*mems2;
	size_t			i;

	mems1 = (unsigned char *)s1;
	mems2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (mems1[i] != mems2[i])
			return (mems1[i] - mems2[i]);
		i++;
	}
	return (0);
}
