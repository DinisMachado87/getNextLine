/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/17 20:18:22 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static int ft_strchr(char *str, int c, ssize_t *char_read)
{
	ssize_t i_chr;
	
	i_chr = 0;
	if (!str)
		return (0);
	while (str[i_chr] && (str)[i_chr] != (char)c)
		(i_chr)++;
	if ((str[i_chr] == (char)c) && ((char)c == '\0'))
	{
		*char_read = i_chr;
		return (*char_read);
	}
	if ((str[i_chr]) && (str[i_chr + 1]) && ((str)[i_chr] == (char)c))
	{
		*char_read = i_chr;
		return (*char_read);
	}
	return (0);
}

char *free_and_null_str(char *str)
{
	if (str)
		free(str);
	return (NULL);
}

static char	*ft_split(char **in_str, char **splt_str, ssize_t splt_pt)
{
	char	*temp_str;
	ssize_t	len;

	len = (ft_strchr(*in_str, '\0', &len) - splt_pt);
	temp_str = (char *)malloc((len + 1) * sizeof(char));
	if (!temp_str)
		return (NULL);
	temp_str[len] = '\0';
	while (0 < len--)
		temp_str[len] = (*in_str)[splt_pt + len];
	*splt_str = temp_str;

	temp_str = (char *)malloc((splt_pt + 1) * sizeof(char));
	if (!temp_str)
		return (NULL);
	temp_str[splt_pt] = '\0';
	while (0 < splt_pt--)
		temp_str[splt_pt] = (*in_str)[splt_pt];
	*in_str = free_and_null_str(*in_str);
	*in_str = temp_str;
	return (*in_str);
}

char	*ft_strjoin(char *str_1, char *str_2)
{
	char *joined_str;
	ssize_t len_1;
	ssize_t len_2;

	if (!str_1 && !str_2)
		return (NULL);
	if (!str_1)
		return (str_2);
	if (!str_2)
		return (str_1);
	ft_strchr(str_1, '\0', &len_1);
	ft_strchr(str_2, '\0', &len_2);
	joined_str = (char *)malloc(len_1 + len_2 + 1);
	if (!joined_str)
		return (NULL);
	len_1 = -1;
	len_2 = 0;
	while ((str_1)[++len_1])
		joined_str[len_1] = (str_1)[len_1];
	while ((str_2)[len_2])
		joined_str[len_1++] = (str_2)[len_2++];
	joined_str[len_1] = '\0';
	str_1 = free_and_null_str(str_1);
	str_2 = free_and_null_str(str_2);
	return (joined_str);
}

char *read_until_new_ln_or_eof(t_fd_nd **fd_nd)
{
	char *read_buffer;

	read_buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!read_buffer)
		return (NULL);
	(*fd_nd)->char_read = read((*fd_nd)->fd, read_buffer, BUFFER_SIZE);
	if ((*fd_nd)->char_read <= 0)
	{
		(*fd_nd)->end = 1;
		read_buffer = free_and_null_str(read_buffer);
		if ((*fd_nd)->ln_buffer)
			return ((*fd_nd)->ln_buffer);
		return (NULL);
	}
	read_buffer[(*fd_nd)->char_read] = '\0';
	if ((*fd_nd)->ln_buffer)
		(*fd_nd)->ln_buffer = ft_strjoin((*fd_nd)->ln_buffer, read_buffer);
	else
	{
		(*fd_nd)->ln_buffer = read_buffer;
		read_buffer = NULL;
	}
	return ((*fd_nd)->ln_buffer);
}

char *build_ln(t_fd_nd **fd_nd, t_fd_nd **fd_head)
{
	char *final_str;

	final_str = NULL;
	while (1)
	{
		(*fd_nd)->ln_buffer = read_until_new_ln_or_eof(fd_nd);
		if ((*fd_nd)->end)
		{
			final_str = ft_strjoin((*fd_nd)->ln_buffer, final_str);
			free_node(fd_nd, fd_head);
			return (final_str);
		}
		if (!(*fd_nd)->ln_buffer)
			return (NULL);
		if (ft_strchr((*fd_nd)->ln_buffer, '\n', &(*fd_nd)->char_read))
			return (ft_split(&(*fd_nd)->ln_buffer, &(*fd_nd)->next_ln, (*fd_nd)->char_read + 1));
	}
}

char	*get_next_line(int fd)
{
	static t_fd_nd *fd_head = NULL;
	t_fd_nd *fd_nd;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	fd_nd = get_or_add_node(fd, &fd_head);
	if (!fd_nd)
		return (NULL);
	if (fd_nd->next_ln)
	{
		fd_nd->ln_buffer = fd_nd->next_ln;
		fd_nd->next_ln = NULL;
		if (ft_strchr(fd_nd->ln_buffer, '\n', &fd_nd->char_read))
			return (ft_split(&fd_nd->ln_buffer, &fd_nd->next_ln, fd_nd->char_read + 1));
	}
	return (build_ln(&fd_nd, &fd_head));
}
