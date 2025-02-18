/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 23:31:28 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/18 04:33:08 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h> // For open()

typedef struct s_line_test
{
	char	*line;
	int		fd;
}	t_line_test;

t_line_test	*add_test(int fd)
{
	t_line_test	*fd_test;

	fd_test = (t_line_test *)malloc(sizeof(t_line_test));
	if (!fd_test)
		return (NULL);
	fd_test->line = NULL;
	fd_test->fd = fd;
	return (fd_test);
}

int	main(int argc, char **argv)
{
	t_line_test	*fd1;
	t_line_test	*fd2;
	int			fd1_done;
	int			fd2_done;

	fd1_done = 0;
	fd2_done = 0;
	if (argc < 3)
		return (-1);
	fd1 = add_test(open(argv[1], O_RDONLY));
	if (!fd1)
		return (-1);
	fd2 = add_test(open(argv[2], O_RDONLY));
	if (!fd2)
		return (-1);
	while (!fd1_done || !fd2_done)
	{
		if (!fd1_done)
		{
			fd1->line = get_next_line(fd1->fd);
			if (fd1->line)
			{
				printf("MAIN:%s", fd1->line);
				free(fd1->line);
			}
			else
				fd1_done = 1;
		}
		if (!fd2_done)
		{
			fd2->line = get_next_line(fd2->fd);
			if (fd2->line)
			{
				printf("MAIN:%s", fd2->line);
				free(fd2->line);
			}
			else
				fd2_done = 1;
		}
	}
	printf("Both Fd over\n");
	close(fd1->fd);
	close(fd2->fd);
	free(fd1);
	free(fd2);
	return (0);
}
