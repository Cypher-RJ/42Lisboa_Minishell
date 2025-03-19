/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:55:08 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/02/05 10:20:29 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/get_next_line.h"

char	*get_next_line(int fd)
{
	static char	file_line [BUFFER_SIZE + 1];
	char		*final_line;
	int			i;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	final_line = NULL;
	while (new_line(final_line) == -1)
	{
		if (file_line[0] == '\0')
		{
			i = read(fd, file_line, BUFFER_SIZE);
			if (i <= 0)
			{
				if (i == 0)
					return (final_line);
				free(final_line);
				return (NULL);
			}
			file_line[i] = '\0';
		}
		final_line = gnl_strjoin(final_line, file_line);
		read_clear(final_line, file_line);
	}
	return (final_line);
}

/* int	main()
{
	int		fd;
	char	*line;
	int		lines;

	lines = 1;
	fd = open("test0.txt", O_RDONLY);

	while ((line = get_next_line(fd)))
	{
		printf("%d->%s\n", lines++, line);
		free(line);
	}
} */