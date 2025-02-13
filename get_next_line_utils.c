/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 21:36:51 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/13 03:58:24 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
** - Frees the node and removes it from the list.
**
** Return: NULL
**/
#include "get_next_line.h"

void	*free_node(t_fd_node *fd_node, t_fd_node **fd_list_head)
{
	if (!fd_node)
		return (NULL);
	if (fd_node->prev_fd_node)
		fd_node->prev_fd_node->next_fd_node = fd_node->next_fd_node;
	else
		*fd_list_head = fd_node->next_fd_node;
	if (fd_node->next_fd_node)
		fd_node->next_fd_node->prev_fd_node = fd_node->prev_fd_node;
	free(fd_node->next_line);
	free(fd_node);
	return (NULL);
}

/**
 ** - Gets the node corresponding to the fd.
 ** - If the node does not exist, it creates a new node.
 ** 
 ** Return: The node corresponding to the fd or a new node.
 */
t_fd_node *get_or_add_node(int fd, t_fd_node **fd_list_head)
{
	t_fd_node *current_node;

	current_node = *fd_list_head;
	while (current_node && (current_node->fd != fd))
		current_node = current_node->next_fd_node;
	if (current_node)
		return (current_node);
	current_node = (t_fd_node *)malloc(sizeof(t_fd_node));
	if (!current_node)
		return (NULL);
	current_node->fd = fd;
	current_node->next_line = NULL;
	current_node->line_buffer = NULL;
	current_node->char_read = 0;
	current_node->next_fd_node = *fd_list_head;
	current_node->prev_fd_node = NULL;
	if (*fd_list_head)
		(*fd_list_head)->prev_fd_node = current_node;
	*fd_list_head = current_node;
	return (current_node);
}
