/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:22:44 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 15:35:38 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			size_tt;
	unsigned char	*ptr;
	size_t			i;

	if (size && nmemb > SIZE_MAX / size)
		return (NULL);
	size_tt = nmemb * size;
	ptr = malloc(size_tt);
	i = 0;
	if (ptr == NULL)
		return (NULL);
	while (i < size_tt)
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
}
