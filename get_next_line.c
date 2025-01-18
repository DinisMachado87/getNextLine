/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2024/12/16 18:54:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
	write(1, "\n", 1);
	return(str);
}

static char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s)
		s++;
	if ((char)c == *s)
		return ((char *)s);
	return (NULL);
}

char	*get_next_line(int fd)
{
	char	*line_buffer;
	int		line_size;
	int		char_read;
	char	*next_line;
	char	read_buffer[BUFFER_SIZE];

	next_line = NULL;
	line_size = 0;
	while (!next_line)
	{
		char_read = read(fd, read_buffer, BUFFER_SIZE);
		if (char_read == -1)
			return (ft_putstr("Error reading file"));
		next_line = ft_strchr(read_buffer, '\n');
		ft_putstr(read_buffer);
		line_size += (BUFFER_SIZE - 1);
		line_buffer = (char *)malloc((line_size + 1) * sizeof(char));
	}
	return (ft_putstr(line_buffer));
}
