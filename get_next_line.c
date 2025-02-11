/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/12 00:18:05 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static char	*ft_split(char **init_str, char **splited_str, ssize_t split_point)
{
	char	*s;
	ssize_t	i_new_line;

	s = *init_str + split_point;
	i_new_line = 0;
	while (s[i_new_line])
		i_new_line++;
	*splited_str = (char *)malloc((i_new_line + 1) * sizeof(char));
	if (!*splited_str)
		return (NULL);
	i_new_line = 0;
	while (s[i_new_line])
	{
		(*splited_str)[i_new_line] = s[i_new_line];
		i_new_line++;
	}
	(*splited_str)[i_new_line] = '\0';
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
	*char_read = i_chr;
	return (0);
}
char	*free_str(char *str)
{
	free(str);
	str = NULL;
	return (NULL);
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

static char *ft_strjoin(
		char *line_buffer,
		ssize_t *line_size,
		char *read_buffer,
		ssize_t char_read)
{
	char *cat_line;
	ssize_t i_line;
	ssize_t i_buffer;

	*line_size += char_read;
	cat_line = ft_calloc(*line_size + 2);
	if (!cat_line)
		return (NULL);
	i_line = 0;
	while (line_buffer && line_buffer[i_line])
	{
		cat_line[i_line] = line_buffer[i_line];
		i_line++;
	}
	if (line_buffer)
		line_buffer = free_str(line_buffer);
	i_buffer = 0;
	while (i_buffer < char_read)
	{
		cat_line[i_line + i_buffer] = read_buffer[i_buffer];
		i_buffer++;
	}
	cat_line[i_line + i_buffer] = '\0';
	return (cat_line);
}

char *handle_error_or_EOF(char **read_buffer, t_fd_node *fd_node, t_fd_node *fd_list_head, char **line_buffer)
{
	free_str(*read_buffer);
	if (fd_node->char_read == 0 && *line_buffer)
	{
		ssize_t len = 0;
		while ((*line_buffer)[len])
			len++;
		if (len > 0 && (*line_buffer)[len - 1] != '\n')
		{
			char *new_line = ft_strjoin(*line_buffer, &len, "\n", 1);
			free_str(*line_buffer);
			*line_buffer = new_line;
		}
		return *line_buffer;
	}

	if (*line_buffer)
		free_str(*line_buffer);
	free_node(fd_node, &fd_list_head);
	return NULL;
}

char *read_until_new_line_or_EOF(
		char **line_buffer,
		ssize_t *line_size,
		t_fd_node *fd_node,
		t_fd_node *fd_list_head)
{
	char *read_buffer;

	read_buffer = ft_calloc(BUFFER_SIZE);
	if (!read_buffer)
		return (free_str(*line_buffer));
	fd_node->char_read = read(fd_node->fd, read_buffer, (BUFFER_SIZE));
	if (fd_node->char_read <= 0)
		return (handle_error_or_EOF(&read_buffer, fd_node, fd_list_head, line_buffer));
	read_buffer[fd_node->char_read] = '\0';
	if (ft_strchr(read_buffer, '\n', &fd_node->char_read))
	{
		ft_split(&read_buffer, &fd_node->next_line, fd_node->char_read);
		*line_buffer = ft_strjoin(*line_buffer, line_size, read_buffer, fd_node->char_read);
		free(read_buffer);
		return (*line_buffer);
	}
	*line_buffer = ft_strjoin(*line_buffer, line_size, read_buffer, fd_node->char_read);
	free(read_buffer);
	return (*line_buffer);
}

char *get_next_line(int fd)
{
	static t_fd_node *fd_list_head = NULL;
	t_fd_node *fd_node;
	char *line_buffer;
	ssize_t line_size;

	line_size = 0;
	fd_node = get_or_add_node(fd, &fd_list_head);
	line_buffer = NULL;
	fd_node->char_read = 0;
	if (fd_node->next_line && (ft_strchr(fd_node->next_line, '\n', &fd_node->char_read)))
	{
		line_buffer = ft_strjoin(line_buffer, &line_size, fd_node->next_line, fd_node->char_read);
		ft_split(&fd_node->next_line, &fd_node->next_line, fd_node->char_read);
		return (line_buffer);
	}
	else
	{
		line_buffer = fd_node->next_line;
		fd_node->next_line = NULL;
	}
	while (!fd_node->next_line)
	{
		line_buffer = read_until_new_line_or_EOF(&line_buffer, &line_size, fd_node, fd_list_head);
		if (!line_buffer)
			return (NULL);
	}
	return (line_buffer);
}
