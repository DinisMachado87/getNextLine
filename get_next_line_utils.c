/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 21:36:51 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/06 03:11:24 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*free_node(struct s_fd_node *fd_node, t_fd_node **fd_list_head)
{
	if (!fd_node)
		return (NULL);
	if (fd_node->next_line)
		free(fd_node->next_line);
	if (fd_node->prev_fd_node)
		fd_node->prev_fd_node->next_fd_node = fd_node->next_fd_node;
	else
		*fd_list_head = fd_node->next_fd_node;
	if (fd_node->next_fd_node)
		fd_node->next_fd_node->prev_fd_node = fd_node->prev_fd_node;
	free(fd_node);
	return (NULL);
}

t_fd_node	*get_or_add_node(int fd, t_fd_node **fd_list_head)
{
	t_fd_node			*fd_node;
	t_fd_node			*current_node;
	t_fd_node			*prev_node;

	current_node = *fd_list_head;
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
	fd_node->next_fd_node = *fd_list_head;
	fd_node->prev_fd_node = NULL;
	if (*fd_list_head)
		(*fd_list_head)->prev_fd_node = prev_node;
	*fd_list_head = fd_node;
	return (fd_node);
}
