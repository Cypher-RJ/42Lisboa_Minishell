/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 19:37:51 by rcesar-d          #+#    #+#             */
/*   Updated: 2024/10/14 15:38:37 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	strlen;
	char	*str;

	i = 0;
	strlen = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[i] && ft_strchr(set, s1[i]) != NULL)
		i++;
	j = ft_strlen(s1);
	while (j > i && ft_strrchr(set, s1[j - 1]) != NULL)
		j--;
	str = malloc(sizeof(char) * (j - i + 1));
	if (!str)
		return (NULL);
	while (i < j)
	{
		str[strlen] = s1[i];
		strlen++;
		i++;
	}
	str[strlen] = '\0';
	return (str);
}
