/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/07 16:25:53y dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static char *ft_split(char *init_str, char *new_str_after_split, ssize_t split_point)
{
	const char *s;
	ssize_t new_line_count;

	s = init_str + split_point;
	new_line_count = 0;

	while (s[new_line_count])
		new_line_count++;
	new_str_after_split = (char *)malloc((new_line_count + 1) * sizeof(char));
	if (!new_str_after_split)
		return (NULL);
	new_line_count = 0;
	while (s[new_line_count])
	{
		new_str_after_split[new_line_count] = s[new_line_count];
		new_line_count++;
	}
	new_str_after_split[new_line_count] = '\0';
	return (init_str);
}

static int ft_strchr(char *init_str, int c, ssize_t *char_read)
{
	ssize_t i_chr;

	i_chr = 0;
	while (init_str[i_chr] && init_str[i_chr] != (char)c)
		i_chr++;
	if ((char)c == init_str[i_chr])
	{
		i_chr++;
		*char_read = i_chr;
		return (1);
	}
	return (0);
}

static char *ft_strcat(char *line_buffer, ssize_t *line_size,
					   char *read_buffer, ssize_t char_read)
{
	char *cat_line;
	ssize_t i_line;
	ssize_t i_buffer;

	*line_size += char_read;
	cat_line = (char *)malloc((*line_size + 1) * sizeof(char));
	if (!cat_line)
		return (NULL);
	i_line = 0;
	while (line_buffer && line_buffer[i_line])
	{
		cat_line[i_line] = line_buffer[i_line];
		i_line++;
	}
	if (line_buffer)
		free(line_buffer);
	i_buffer = 0;
	while (i_buffer < char_read)
	{
		cat_line[i_line + i_buffer] = read_buffer[i_buffer];
		i_buffer++;
	}
	cat_line[i_line + i_buffer] = '\0';
	return (cat_line);
}

void *ft_calloc(ssize_t str_size)
{
	char *str_ptr;
	ssize_t i_ltr;

	i_ltr = 0;
	str_ptr = (char *)malloc((str_size + 1) * sizeof(char));
	if (!str_ptr)
		return (NULL);
	while (i_ltr <= (str_size))
	{
		str_ptr[i_ltr] = '\0';
		i_ltr++;
	}
	return (str_ptr);
}
char *get_next_line(int fd)
{
	/*		- so that it is reusable here to handle cases of several consecutive new lines
				- returning imidiatelly new Line untill the '\n'
				- and setting new line with the remaining string:
	*/
	static t_fd_node	*fd_list_head = NULL;
	t_fd_node			*fd_node;
	char				*line_buffer;
	ssize_t				char_read;
	char				*read_buffer;
	ssize_t				line_size;

	fd_node = get_or_add_node(fd, &fd_list_head);
	line_size = 0;
	line_buffer = NULL;
	while (ft_strchr(fd_node->next_line, '\n', &char_read))
	{
		read_buffer = fd_node->next_line;
		return (ft_split(read_buffer, fd_node->next_line, char_read));
	}
	line_buffer = fd_node->next_line;
	fd_node->next_line = NULL;
	while (!fd_node->next_line)
	{
		read_buffer = ft_calloc(BUFFER_SIZE);
		if (!read_buffer)
		{
			if (line_buffer)
				free(line_buffer);
			return (NULL);
		}
		char_read = read(fd, read_buffer, BUFFER_SIZE);
		if (char_read <= 0)
		{
			free(read_buffer);
			if (char_read == 0 && line_buffer && (line_buffer[0] != '\0'))
				return (line_buffer);
			if (line_buffer)
				free(line_buffer);
			fd_node = free_node(fd_node, &fd_list_head);
			return (NULL);
		}
		if (ft_strchr(read_buffer, '\n', &char_read))
			ft_split(read_buffer, fd_node->next_line, char_read);
		read_buffer[char_read] = '\0';
		line_buffer = ft_strcat(line_buffer, &line_size, read_buffer, char_read);
		free(read_buffer);
	}
	return (line_buffer);
}
/*
void print_test_result(int result, const char *test_name)
	{
		if (result)
			printf("\033[0;32m%s - OK\033[0m\n\n", test_name);
		else
			printf("\033[0;31m%s - KO\033[0m\n\n", test_name);
	}

void	test_ft_split(void)
{
	char	*str = "Hello World!";
	char	*new_str = NULL;
	ssize_t	split_point;

	printf("Test ft_split\n\n");
	printf("Test 1 - Split at the middle\n");
	split_point = 5;
	if (ft_split(str, new_str, split_point) == str && strcmp(new_str, " World!") == 0)
		print_test_result(1, "Test 1");
	else
		print_test_result(0, "Test 1");

	printf("Test 2 - Split at the end\n");
	split_point = 11;
	if (ft_split(str, new_str, split_point) == str && strcmp(new_str, "") == 0)
		print_test_result(1, "Test 2");
	else
		print_test_result(0, "Test 2");

	printf("Test 3 - Split at the beginning\n");
	split_point = 0;
	if (ft_split(str, new_str, split_point) == str && strcmp(new_str, "Hello World!") == 0)
		print_test_result(1, "Test 3");
	else
		print_test_result(0, "Test 3");
}

void	test_ft_strchr(void)
{
	char	*str = "Hello World!";
	ssize_t	char_read;

	printf("Test ft_strchr\n\n");
	printf("Test 1 - char found\n");


	if (ft_strchr(str, 'W', &char_read) == 1)
		print_test_result(1, "Test 1");
	else
		print_test_result(0, "Test 1");

	printf("Test 2 - char not found\n");
	if (ft_strchr(str, 'w', &char_read) == 0)
		print_test_result(1, "Test 2");
	else
		print_test_result(0, "Test 2");

	printf("Test 3 - char found at the end\n");
	if (ft_strchr(str, '!', &char_read) == 1)
		print_test_result(1, "Test 3");
	else
		print_test_result(0, "Test 3");
}



void	test_ft_strcat(void)
{
	char	*str1 = "Hello ";
	char	*str2 = "World!";
	char	*result;
	ssize_t	line_size;

	printf("Test ft_strcat\n\n");
	printf("Test 1 - Concatenate two strings\n");
	line_size = 0;
	result = ft_strcat(str1, &line_size, str2, 7);
	if (strcmp(result, "Hello World!") == 0)
		print_test_result(1, "Test 1");
	else
		print_test_result(0, "Test 1");
	free(result);

	printf("Test 2 - Concatenate two NULL strings\n");
	result = ft_strcat(NULL, &line_size, NULL, 0);
	if (strcmp(result, "") == 0)
		print_test_result(1, "Test 2");
	else
		print_test_result(0, "Test 2");
	free(result);

	printf("Test 3 - Concatenate an empty string with one NULL\n");
	result = ft_strcat(NULL, &line_size, str2, 7);
	if (strcmp(result, "World!") == 0)
		print_test_result(1, "Test 3");
	else
		print_test_result(0, "Test 3");
	free(result);

	printf("Test 4 - Concatenate a non-empty string with one NULL\n");
	result = ft_strcat(str1, &line_size, NULL, 0);
	if (strcmp(result, "Hello ") == 0)
		print_test_result(1, "Test 4");
	else
		print_test_result(0, "Test 4");
	free(result);

	printf("Test 5 - Concatenate two strings with one empty\n");
	result = ft_strcat(str1, &line_size, "", 0);
	if (strcmp(result, "Hello ") == 0)
		print_test_result(1, "Test 5");
	else
		print_test_result(0, "Test 5");
	free(result);

	printf("Test 6 - Concatenate two strings with one empty\n");
	result = ft_strcat("", &line_size, str2, 7);
	if (strcmp(result, "World!") == 0)
		print_test_result(1, "Test 6");
	else
		print_test_result(0, "Test 6");
	free(result);

	printf("Test 7 - Concatenate two strings empty strings\n");
	result = ft_strcat("", &line_size, "", 0);
	if (strcmp(result, "") == 0)
		print_test_result(1, "Test 7");
	else
		print_test_result(0, "Test 7");
	free(result);
}

int		main(void)
{
	test_ft_strchr();
	test_ft_split();
	return (0);
}
*/