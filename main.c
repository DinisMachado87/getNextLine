/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 23:31:28 by dimachad          #+#    #+#             */
/*   Updated: 2025/01/23 05:22:31 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

typedef struct s_line_test
{
	char	*line;
	int		fd;
} t_line_test;

t_line_test	*add_test(int fd)
{
	static t_line_test	*fd_test;

	fd_test = (t_line_test *)malloc(sizeof(t_line_test));
	fd_test->line = NULL;
	fd_test->fd = fd;

	return (fd_test);
}

int	main(int argc, char **argv)
{
	t_line_test	*fd1;
	t_line_test	*fd2;

	if (argc < 3)
		return (-1);
	fd1 = add_test(open(argv[1], O_RDONLY));
	if (!fd1)
		return (-1);
	fd2 = add_test(open(argv[2], O_RDONLY));
	if (!fd2)
		return (-1);
	fd1->line = get_next_line(fd1->fd); 
	fd2->line = get_next_line(fd2->fd);
	while (fd1->line || fd2->line)
	{
		fd1->line = get_next_line(fd1->fd); 
		fd2->line = get_next_line(fd2->fd);
		if (fd1->line)
			printf("%s", fd1->line);
		if (fd2->line)
			printf("%s", fd2->line);
	}
	close(fd1->fd);
	close(fd2->fd);
	return (0);
}
