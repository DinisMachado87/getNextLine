/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/11 03:52:25 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static char *ft_split(char **init_str, char **new_str_after_split, ssize_t split_point)
{
	char *s;
	ssize_t new_line_count;

	s = *init_str + split_point;
	new_line_count = 0;

	while (s[new_line_count])
		new_line_count++;
	*new_str_after_split = (char *)malloc((new_line_count + 1) * sizeof(char));
	if (!*new_str_after_split)
		return (NULL);
	new_line_count = 0;
	while (s[new_line_count])
	{
		(*new_str_after_split)[new_line_count] = s[new_line_count];
		new_line_count++;
	}
	(*new_str_after_split)[new_line_count] = '\0';
	(*init_str)[split_point] = '\0';
	return (*init_str);
}

static int ft_strchr(char *init_str, int c, ssize_t *char_read)
{
	ssize_t i_chr;

	i_chr = 0;
	while (init_str[i_chr] && init_str[i_chr] != (char)c)
		i_chr++;
	if ((char)c == init_str[i_chr])
	{
		*char_read = i_chr + 1;
		return (1);
	}
	return (0);
}

static char *ft_strcat(char *line_buffer, ssize_t *line_size,
					   char *read_buffer, ssize_t char_read)
{
	char *cat_line;
	ssize_t i_line;
	ssize_t i_buffer;

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

void *ft_calloc(ssize_t str_size)
{
	char *str_ptr;
	ssize_t i_ltr;

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
char *get_next_line(int fd)
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
	char_read = 0;

	if (fd_node->next_line && !(ft_strchr(fd_node->next_line, '\n', &char_read)))
	{
		printf("char_read %zd", char_read);
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
		char_read = read(fd, read_buffer, (BUFFER_SIZE));
		if (char_read <= 0)
		{
			free(read_buffer);
			if (char_read == 0 && line_buffer && (line_buffer[0] != '\0'))
				return (line_buffer);
			if (line_buffer)
				free(line_buffer);
			fd_node = free_node(fd_node, &fd_list_head);
			return (NULL);
		}
		read_buffer[char_read] = '\0';
		if (ft_strchr(read_buffer, '\n', &char_read))
		{
			ft_split(&read_buffer, &fd_node->next_line, char_read);
			free(read_buffer);
			return (line_buffer);
		}
		line_buffer = ft_strcat(line_buffer, &line_size, read_buffer, char_read);
		free(read_buffer);
	}
	return (line_buffer);
}
