/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 21:36:51 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/19 00:10:11 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*free_node(t_fd_nd **fd_nd, t_fd_nd **fd_head)
{
	if (!(*fd_nd))
		return (NULL);
	if ((*fd_nd)->prev_fd_nd)
		(*fd_nd)->prev_fd_nd->next_fd_nd = (*fd_nd)->next_fd_nd;
	else
		(*fd_head) = (*fd_nd)->next_fd_nd;
	if ((*fd_nd)->next_fd_nd)
		(*fd_nd)->next_fd_nd->prev_fd_nd = (*fd_nd)->prev_fd_nd;
	if ((*fd_nd)->next_ln)
		(*fd_nd)->next_ln = free_and_null_str((*fd_nd)->next_ln);
	free(*fd_nd);
	return (NULL);
}

t_fd_nd	*get_or_add_node(int fd, t_fd_nd **fd_head)
{
	t_fd_nd	*curr_nd;

	curr_nd = *fd_head;
	while (curr_nd && (curr_nd->fd != fd))
		curr_nd = curr_nd->next_fd_nd;
	if (curr_nd)
		return (curr_nd);
	curr_nd = (t_fd_nd *)malloc(sizeof(t_fd_nd));
	if (!curr_nd)
		return (NULL);
	curr_nd->fd = fd;
	curr_nd->next_ln = NULL;
	curr_nd->char_read = 0;
	curr_nd->next_fd_nd = *fd_head;
	curr_nd->prev_fd_nd = NULL;
	curr_nd->end = 0;
	if (*fd_head)
		(*fd_head)->prev_fd_nd = curr_nd;
	*fd_head = curr_nd;
	return (curr_nd);
}

char	*free_and_null_str(char *str)
{
	if (str)
		free(str);
	str = NULL;
	return (NULL);
}
