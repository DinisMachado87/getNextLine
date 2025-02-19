/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/19 04:03:51 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
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
	ssize_t			chr_read;
	char			*next_ln;
	int				end;
}	t_fd_nd;

t_fd_nd	*get_or_add_node(int fd, t_fd_nd **fd_list_head);
void	*free_node(struct s_fd_nd **fd_nd, t_fd_nd **fd_list_head);
int		find_nl_or_nt(char *str, int c, ssize_t *chr_read);
char	*get_next_line(int fd);
char	*free_and_null_str(char *str);
void	ft_strcpy(char *dest, char *src);

#endif
