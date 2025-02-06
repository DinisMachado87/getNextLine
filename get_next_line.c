/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/06 05:03:37 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static char	*ft_strchr(char *init_str, int c, ssize_t *char_read)
{
	const char	*s;
	char		*new_line;
	ssize_t		new_line_count;

	new_line = NULL;
	s = init_str;
	new_line_count = 0;
	while (*s && *s != (char)c)
		s++;
	if ((char)c == *s)
	{
		s++;
		*char_read = (ssize_t)(s - init_str);
		while (s[new_line_count])
			new_line_count++;
		new_line = (char *)malloc((new_line_count + 1) * sizeof(char));
		if (!new_line)
			return (NULL);
		new_line_count = 0;
		while (s[new_line_count])
		{
			new_line[new_line_count] = s[new_line_count];
			new_line_count++;
		}
		new_line[new_line_count] = '\0';
		return (new_line);
	}
	return (NULL);
}

static char	*ft_strcat(char *line_buffer, ssize_t *line_size,
	char *read_buffer, ssize_t char_read)
{
	char	*cat_line;
	ssize_t	i_line;
	ssize_t	i_buffer;

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
	if (line_buffer)
		free(line_buffer);
	i_buffer = 0;
	while (i_buffer < char_read)
	{
		cat_line[i_line + i_buffer] = read_buffer[i_buffer];
		i_buffer++;
	}
	cat_line[i_line + i_buffer] = '\0';
	return (cat_line);
}

void	*ft_calloc(ssize_t str_size)
{
	char	*str_ptr;
	ssize_t	i_ltr;

	i_ltr = 0;
	str_ptr = (char *)malloc((str_size + 1) * sizeof(char));
	if (!str_ptr)
		return (NULL);
	while (i_ltr <= (str_size))
	{
		str_ptr[i_ltr] = '\0';
		i_ltr++;
	}
	return (str_ptr);
}

char	*get_next_line(int fd)
{
	static t_fd_node	*fd_list_head = NULL;
	t_fd_node			*fd_node;
	char				*line_buffer;
	ssize_t				char_read;
	char				*read_buffer;
	ssize_t				line_size;

	fd_node = get_or_add_node(fd, &fd_list_head);
	line_size = 0;
	line_buffer = NULL;
	if (fd_node->next_line)
	{

/*	- Refactor ft_str_chr into separated functions:
			- one that checks for the index of char c if it exists 
			- and (SPLIT)another that actually separates the string in two
	- so that it is reusable here to handle cases of several consecutive new lines
			- returning imidiatelly new Line untill the '\n' 
			- and setting new line with the remaining string:
*/	
//		while (fd_node->next_line)
//		{
//			fd_node->next_line++;
//			return ("\n");
//		}
		line_buffer = fd_node->next_line; 
		fd_node->next_line = NULL;
	}
	while (!fd_node->next_line)
	{
		read_buffer = ft_calloc(BUFFER_SIZE);
		if (!read_buffer)
		{
			if (line_buffer)
				free(line_buffer);
			return (NULL);
		}
		char_read = read(fd, read_buffer, BUFFER_SIZE);
		if (char_read <= 0)
		{
			free(read_buffer);
			if (char_read == 0 && line_buffer && (line_buffer[0] != '\0'))
				return (line_buffer);
			if (line_buffer)
				free (line_buffer);
			fd_node = free_node(fd_node, &fd_list_head);
			return (NULL);
		}
		read_buffer[char_read] = '\0';
		fd_node->next_line = ft_strchr(read_buffer, '\n', &char_read);
		line_buffer = ft_strcat(line_buffer, &line_size,
				read_buffer, char_read);
		free (read_buffer);
	}
	return (line_buffer);
	}
