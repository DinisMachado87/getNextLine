#include "get_next_line.h"
#include <stdio.h>

int		main(int argc, char **argv)
{
	int	fd;

	if (argc < 2)
		return (-1);
	fd = open(argv[1], O_RDONLY);
	if (!fd)
		return (-1);
	
	printf("fd created with code %d\n", fd);
	
	get_next_line(fd);
	close(fd);
	return (0);
}
