/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/18 04:32:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 30
# endif

# include <unistd.h>
# include <strings.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct s_fd_nd
{
	int				fd;
	struct s_fd_nd	*next_fd_nd;
	struct s_fd_nd	*prev_fd_nd;
	ssize_t			char_read;
	char			*next_ln;
	int				end;
}	t_fd_nd;

char	*get_next_line(int fd);
void	*free_node(struct s_fd_nd **fd_nd, t_fd_nd **fd_list_head);
t_fd_nd	*get_or_add_node(int fd, t_fd_nd **fd_list_head);
char	*free_and_null_str(char *str);

#endif
