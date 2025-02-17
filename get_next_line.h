/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/17 15:48:02 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 8
# endif

# include <unistd.h>
# include <strings.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct s_fd_nd
{
	int					fd;
	struct s_fd_nd 	*next_fd_nd;
	struct s_fd_nd	*prev_fd_nd;
	ssize_t 			char_read;
	char				*next_line;
	char				*line_buffer;
	int					end;
}	t_fd_nd;

char		*get_next_line(int fd);
void		*free_node(struct s_fd_nd **fd_nd, t_fd_nd **fd_list_head);
t_fd_nd	*get_or_add_node(int fd, t_fd_nd **fd_list_head);


#endif
