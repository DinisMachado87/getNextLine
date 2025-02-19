/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/19 02:44:37 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_split(char **in_str, char **split_str, ssize_t splt_pt)
{
	char	*temp_str;
	ssize_t	len;

	len = (find_nl_or_nt(*in_str, '\0', &len) - splt_pt);
	temp_str = (char *)malloc((len + 1) * sizeof(char));
	if (!temp_str)
		return (NULL);
	temp_str[len] = '\0';
	while (0 < len--)
		temp_str[len] = (*in_str)[splt_pt + len];
	*split_str = temp_str;
	temp_str = (char *)malloc((splt_pt + 1) * sizeof(char));
	if (!temp_str)
		return (free_and_null_str(*split_str));
	temp_str[splt_pt] = '\0';
	while (0 < splt_pt--)
		temp_str[splt_pt] = (*in_str)[splt_pt];
	*in_str = free_and_null_str(*in_str);
	*in_str = temp_str;
	temp_str = NULL;
	return (*in_str);
}

char	*ft_strjoin(char *str_1, char *str_2)
{
	char	*joined_str;
	ssize_t	len_1;
	ssize_t	len_2;

	find_nl_or_nt(str_1, '\0', &len_1);
	find_nl_or_nt(str_2, '\0', &len_2);
	joined_str = malloc(len_1 + len_2 + 1);
	if (!joined_str)
	{
		free_and_null_str(str_1);
		free_and_null_str(str_2);
		return (NULL);
	}
	if (str_1)
		ft_strcpy(joined_str, str_1);
	else
		joined_str[0] = '\0';
	if (str_2)
		ft_strcpy(joined_str + len_1, str_2);
	free_and_null_str(str_1);
	free_and_null_str(str_2);
	return (joined_str);
}

char	*read_until_new_ln_or_eof(t_fd_nd **fd_nd, char **line_buf)
{
	char	*read_buff;

	read_buff = (char *)malloc(BUFFER_SIZE + 1);
	if (!read_buff)
		return (NULL);
	(*fd_nd)->chr_read = read((*fd_nd)->fd, read_buff, BUFFER_SIZE);
	if ((*fd_nd)->chr_read <= 0)
	{
		(*fd_nd)->end = 1;
		read_buff = free_and_null_str(read_buff);
		if ((*fd_nd)->chr_read < 0 || !(*line_buf) || **line_buf == '\0')
		{
			*line_buf = free_and_null_str(*line_buf);
			return (NULL);
		}
		return (*line_buf);
	}
	read_buff[(*fd_nd)->chr_read] = '\0';
	*line_buf = ft_strjoin(*line_buf, read_buff);
	read_buff = NULL;
	return (*line_buf);
}

char	*build_ln(t_fd_nd **fd_nd, t_fd_nd **fd_head, char **line_buf)
{
	while (1)
	{
		*line_buf = read_until_new_ln_or_eof(fd_nd, line_buf);
		if (*line_buf && (find_nl_or_nt(*line_buf, '\n', &(*fd_nd)->chr_read)))
			return (ft_split(line_buf, &(*fd_nd)->next_ln, (*fd_nd)->chr_read));
		if ((*fd_nd)->end)
		{
			free_node(fd_nd, fd_head);
			return (*line_buf);
		}
	}
}

char	*get_next_line(int fd)
{
	static t_fd_nd	*fd_head = NULL;
	t_fd_nd			*fd_nd;
	char			*line_buf;

	line_buf = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	fd_nd = get_or_add_node(fd, &fd_head);
	if (!fd_nd)
		return (NULL);
	if (fd_nd->next_ln)
	{
		line_buf = fd_nd->next_ln;
		fd_nd->next_ln = NULL;
		if (find_nl_or_nt(line_buf, '\n', &fd_nd->chr_read))
			return (ft_split(&line_buf, &fd_nd->next_ln, fd_nd->chr_read));
	}
	return (build_ln(&fd_nd, &fd_head, &line_buf));
}
