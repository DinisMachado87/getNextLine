/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/03 19:17:45 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

t_fd_node	*fd_list_head = NULL;

static char	*ft_strchr(char *s, int c, size_t *char_read)
{
	const char	*init_str;

	init_str = s;
	while (*s != (char)c && *s)
		s++;
	if ((char)c == *s)
	{
		*char_read = (size_t)(s - init_str + 1);
		return ((char *)++s);
	}
	return (NULL);
}

static char	*ft_strcat(char *line_buffer, size_t *line_size, char *read_buffer, size_t char_read)
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
	while (i_buffer < char_read)
	{
		cat_line[i_line + i_buffer] = read_buffer[i_buffer];
		i_buffer++;
	}
	cat_line[i_line + i_buffer] = '\0';
	printf("%s\n", cat_line);
	if(line_buffer)
	{
		free(line_buffer);
		line_buffer = NULL;
	}
	return (cat_line);
}

t_fd_node	*get_or_add_node(int fd)
{
	static t_fd_node	*fd_node;
	t_fd_node			*current_node;
	t_fd_node			*prev_node;

	current_node = fd_list_head;
	prev_node = NULL;
	while (current_node && (current_node->fd != fd))
	{
		prev_node = current_node;
		current_node = current_node->next_fd_node;
	}
	if (current_node)
		return (current_node);
	fd_node = (t_fd_node *)malloc(sizeof(t_fd_node));
	if (!fd_node)
		return (NULL);
	fd_node->fd = fd;
	fd_node->next_line = NULL;
	fd_node->next_fd_node = fd_list_head;
	fd_node->prev_fd_node = prev_node;
	fd_list_head = fd_node;
	return (fd_node);
}

void	free_node(struct s_fd_node *fd_node)
{
	fd_node->prev_fd_node->next_fd_node = fd_node->next_fd_node;
	free(fd_node);
	fd_node = NULL;
}

char	*get_next_line(int fd)
{
	t_fd_node	*fd_node;
	char		*line_buffer;
	size_t		char_read;
	char		read_buffer[BUFFER_SIZE];
	size_t		line_size;
	
	fd_node = get_or_add_node(fd);
	line_size = 0;
	line_buffer = NULL;
	if (fd_node->next_line)
		line_buffer = fd_node->next_line; 
	fd_node->next_line = NULL;
	while (!fd_node->next_line)
	{
		char_read = read(fd, read_buffer, BUFFER_SIZE);
		if (((int)char_read == 0) || ((int)char_read == -1))
		{
			if (fd_node)
				free_node(fd_node);
			return (NULL);
		}
		fd_node->next_line = ft_strchr(read_buffer, '\n', &char_read);
		line_buffer = ft_strcat(line_buffer, &line_size, read_buffer, char_read);
		if (fd_node->next_line)
			return (line_buffer);
	}
	return (line_buffer);
}
