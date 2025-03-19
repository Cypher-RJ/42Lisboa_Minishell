/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:29:55 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/02/05 10:21:28 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/get_next_line.h"

int	gnl_strlen(char *c)
{
	int	i;

	i = 0;
	if (c)
	{
		while (c[i])
			i++;
	}
	return (i);
}

char	*gnl_strjoin(char *s1, char *s2)
{
	int		lens1;
	int		lens2;
	int		i;
	int		j;
	char	*str;

	lens1 = gnl_strlen(s1);
	lens2 = gnl_strlen(s2);
	str = (char *)malloc(lens1 + lens2 + 1);
	if (str == NULL)
		return (NULL);
	i = -1;
	j = -1;
	while (i++ < (lens1 -1))
		str[i] = s1[i];
	while (j++ < (lens2 - 1))
	{
		str[i] = s2[j];
		i++;
	}
	str[i] = '\0';
	if (s1)
		free (s1);
	return (str);
}

int	new_line(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

void	*read_clear(char *final_line, char *file_line)
{
	size_t	i;
	int		flag;

	i = 0;
	flag = new_line(final_line);
	if (flag != -1)
	{
		final_line [flag + 1] = '\0';
		flag = new_line (file_line);
		flag++;
		while (file_line[flag])
			file_line[i++] = file_line[flag++];
	}
	while (i < BUFFER_SIZE)
		file_line[i++] = '\0';
	return (final_line);
}
