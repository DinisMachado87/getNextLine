/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 21:36:51 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/17 15:47:30 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

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
	free((*fd_nd)->next_line);
	free(*fd_nd);
	return (NULL);
}

t_fd_nd *get_or_add_node(int fd, t_fd_nd **fd_head)
{
	t_fd_nd *curr_n;

	curr_n = *fd_head;
	while (curr_n && (curr_n->fd != fd))
		curr_n = curr_n->next_fd_nd;
	if (curr_n)
		return (curr_n);
	curr_n = (t_fd_nd *)malloc(sizeof(t_fd_nd));
	if (!curr_n)
		return (NULL);
	curr_n->fd = fd;
	curr_n->next_line = NULL;
	curr_n->line_buffer = NULL;
	curr_n->char_read = 0;
	curr_n->next_fd_nd = *fd_head;
	curr_n->prev_fd_nd = NULL;
	curr_n->end = 0;
	if (*fd_head)
		(*fd_head)->prev_fd_nd = curr_n;
	*fd_head = curr_n;
	return (curr_n);
}
