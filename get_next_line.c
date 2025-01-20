/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/01/20 18:21:33 by dimachad         ###   ########.fr       */
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

char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s)
		s++;
	if ((char)c == *s)
		return ((char *)s);
	return (NULL);
}

static char *ft_strcat(char *line_buffer, size_t *line_size, char *read_buffer, size_t  char_read)
{
	char	*cat_line;
	size_t	i_line;
	size_t	i_buffer;

	*line_size += char_read;
	cat_line = (char *)malloc((*line_size + 1) * sizeof(char));
	if (!cat_line)
		return (NULL);

	i_line = 0;
	while (line_buffer && line_buffer[i_line])
	{
		cat_line[i_line] = line_buffer[i_line];
		i_line++;
	}

	i_buffer = 0;
	while (i_buffer <= char_read)
	{
		cat_line[i_line + i_buffer] = read_buffer[i_buffer];
		i_buffer++;
	}
	cat_line[i_line + i_buffer] = '\0';
	free(line_buffer);
	return (cat_line);
}

char	*get_next_line(int fd)
{
	char	*line_buffer;
	size_t	char_read;
	static char	*next_line;
	char	read_buffer[BUFFER_SIZE];
	size_t	line_size;

	line_size = 0;
	line_buffer = NULL;
	if (next_line)
		line_buffer = next_line; 
	next_line = NULL;
	while (!next_line)
	{
		char_read = read(fd, read_buffer, BUFFER_SIZE);
		if ((int)char_read == -1)
			return (ft_putstr("Error reading file"));
		next_line = ft_strchr(read_buffer, '\n');

		if (next_line)
			ft_putstr(next_line);

		line_buffer = ft_strcat(line_buffer, &line_size, read_buffer, char_read);
		if (next_line)
			return (line_buffer);
		ft_putstr(line_buffer);
	}
	return (line_buffer);
}
