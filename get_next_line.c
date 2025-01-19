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

static void	ft_strcat(char **line_buffer, size_t *line_size, char *read_buffer, size_t  char_read)
{
	char	*cat_line;
	size_t	i_line;
	size_t	i_buffer;

	*line_size += char_read;
	cat_line = (char *)malloc((*line_size + 1) * sizeof(char));
	if (!cat_line)
		return ;

	i_line = 0;
	if (*line_buffer)
	{
		while (*line_buffer)
		{
			cat_line[i_line] = *(line_buffer[i_line]);
			i_line++;
		}
	}

	i_buffer = 0;
	while (i_buffer < char_read)
		cat_line[i_line++] = read_buffer[i_buffer++];
	free(*line_buffer);
	*line_buffer = cat_line;
}

char	*get_next_line(int fd)
{
	char	*line_buffer;
	size_t	char_read;
	char	*next_line;
	char	read_buffer[BUFFER_SIZE];
	size_t	line_size;

	line_size = 0;
	line_buffer = NULL;
	next_line = NULL;
	while (!next_line)
	{
		char_read = read(fd, read_buffer, BUFFER_SIZE);
		if ((int)char_read == -1)
			return (ft_putstr("Error reading file"));
		next_line = ft_strchr(read_buffer, '\n');
		ft_putstr(read_buffer);
		if (!next_line)
			ft_strcat(&line_buffer, &line_size, read_buffer, char_read);
		ft_putstr(line_buffer);
	}
	return (ft_putstr(line_buffer));
}
