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

static int	print_err(char *str)
{
	while (*str)
	{
		write(1, str++, 1);
	}
	write(1, "\n", 1);
	return (-1);
}
/*
static char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s)
		s++;
	if ((char)c == *s)
		return ((char *)s);
	return (NULL);
}
*/
char	*get_next_line(int fd)
{
	char	*line_buffer;
	int		line_size;
	int		char_read;
	char	*read_buffer[BUFFER_SIZE];

	line_size = 0;
//	while ()
	line_size += BUFFER_SIZE;
	line_buffer = (char *)malloc(line_size * sizeof(char));
	char_read = read(fd, read_buffer, line_size - 1);
	if (char_read == -1)
		print_err("Error reading file");

	return (line_buffer);
}

#include <stdio.h>

int		main(int argc, char **argv)
{
	int	fd;

	if (argc < 2)
		return (-1);
	fd = open(argv[1], O_RDONLY);
	if (!fd)
		return (-1);
	
	printf("fd created with code %d\n", fd);
	print_err("Print error test");
	
	get_next_line(fd);
	close(fd);
	return (0);
}
