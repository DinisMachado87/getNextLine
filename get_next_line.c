/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:16:11 by dimachad          #+#    #+#             */
/*   Updated: 2025/02/12 03:16:56by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

/*
** Modified ft_strchr
** returns 1 if the character is found and 0 if it is not.
** Updates the value of char_read to the index after the character found,
** or the string length (index of the null terminator)
** if the character is not found.
**
** Return: 1 if the character is found, 0 if it is not,
** working as a bollean for strlen and strchar.
*/
static int ft_strchr(char *str, int c, ssize_t *char_read)
{
	ssize_t i_chr;
	
	i_chr = 0;
	if (!str)
	{
		*char_read = 0;
		return (0);
	}
	while (str[i_chr] && str[i_chr] != (char)c)
		i_chr++;
	if ((char)c == str[i_chr])
	{
		*char_read = i_chr;
		return (1);
	}
	*char_read = i_chr;
	return (0);
}

/**
 ** Frees the string
 ** and returns NULL back to the pointer in the calling function.
 ** 
 ** Return: NULL
 */
char *free_and_null_str(char *str)
{
	if (str)
		free(str);
	return (NULL);
}

/**
** Splits the string at the splt_pt index.
** Allocates memory for the spltd_str and copies the string from the splt_pt to the end.
** Updates the in_str to end at the splt_pt.
**
** Return: Updated in_str.
*/
static char *ft_split(char **in_str, char **spltd_str, ssize_t splt_pt)
{
	char *s;
	ssize_t i_new_line;

	if (!*in_str || !(*in_str)[0])
		return(free_and_null_str(*in_str));
	if (**in_str + splt_pt == '\0')
	{
		*spltd_str = free_and_null_str(*spltd_str);
		return (*in_str);
	}
	s = *in_str + splt_pt;
	i_new_line = 0;
	while (s[i_new_line])
		i_new_line++;
	*spltd_str = (char *)malloc((i_new_line + 1) * sizeof(char));
	if (!*spltd_str)
		return (NULL);
	i_new_line = 0;
	while (s[i_new_line])
	{
		(*spltd_str)[i_new_line] = s[i_new_line];
		i_new_line++;
	}
	(*spltd_str)[i_new_line] = '\0';
	(*in_str)[splt_pt] = '\0';
	return (*in_str);
}
/**
 ** Joins two strings.
 ** Allocates memory for the joined_str and copies the first string to it.
 ** Frees the first string.
 ** Copies the second string to the joined_str.
 ** Frees the second string.
 **
 ** Return: The joined string.
 */
char	*ft_strjoin(char **str_1, char **str_2)
{
	char *joined_str;
	ssize_t i_chr;
	ssize_t i_line;
	ssize_t len_1;
	ssize_t len_2;

	ft_strchr(*str_1, '\0', &len_1);
	ft_strchr(*str_2, '\0', &len_2);
	joined_str = (char *)malloc(len_1 + len_2 + 1);
	if (!joined_str)
		return (NULL);
	i_line = 0;
	i_chr = 0;
	while (**str_1 && ((*str_1)[i_chr]))
		joined_str[i_line++] = (*str_1)[i_chr++];
	i_chr = 0;
	while (**str_1 && ((*str_2)[i_chr]))
		joined_str[i_line++] = (*str_2)[i_chr++];
	joined_str[i_line] = '\0';
	*str_1 = free_and_null_str(*str_1);
	*str_2 = free_and_null_str(*str_2);
	return (joined_str);
}

/**
 ** checks if there are still characters in the line_buffer.
 ** If there are, it checks if there is a new line character.
 ** If there isnt, it terminates the line with a new line character.
 ** and returns the line_buffer.
 ** otherwise, it frees the line_buffer and returns NULL.
 **
 ** Return: The line_buffer or NULL.
 */
char	*handle_error_or_EOF(char **read_buffer, t_fd_node *fd_node, t_fd_node **fd_list_head)
{
	char *final_str;

	final_str = NULL;
	*read_buffer = free_and_null_str(*read_buffer);
	if (fd_node->line_buffer)
	{
		final_str = ft_strjoin(&fd_node->line_buffer, &final_str);
		return (fd_node->line_buffer);
	}
	free_node(fd_node, fd_list_head);
	return (NULL);
}

/**
 ** Reads from the file descriptor until a '\n' is found.
 ** If a '\n' is found, it splits the line_buffer at the '\n'.
 ** If the end of file is reached, it calls handle_error_or_EOF.
 ** Otherwise, it joins the read_buffer to the line_buffer and continues reading.
 **
 ** Return: The line_buffer or NULL.
 */
char	*read_until_new_line_or_EOF(t_fd_node *fd_node, t_fd_node **fd_list_head)
{
	char *read_buffer;

	read_buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!read_buffer)
		return (NULL);
	fd_node->char_read = read(fd_node->fd, read_buffer, BUFFER_SIZE);
	if (fd_node->char_read <= 0)
		return (handle_error_or_EOF(&read_buffer, fd_node, fd_list_head));
	read_buffer[fd_node->char_read] = '\0';
	if (fd_node->line_buffer)
		fd_node->line_buffer = ft_strjoin(&fd_node->line_buffer, &read_buffer);
	else
	{
		fd_node->line_buffer = read_buffer;
		read_buffer = NULL;
	}
//	if (ft_strchr(fd_node->line_buffer, '\n', &fd_node->char_read))
//		return (ft_split(&fd_node->line_buffer, &fd_node->next_line, (fd_node->char_read + 1)));
	return (fd_node->line_buffer);
}

/**
 ** - Gets the node corresponding to the fd.
 ** If the node does not exist, it creates a new node.
 ** - if there are next_line chars left after the '\n' from the previous call
 ** copies next_line to the line_buffer.
 ** - it checks if there is a '\n' in the line_buffer,
 ** extracts the second part after the '\n' and stores it in next_line,
 ** and returns the first part.
 ** - Otherwise, it read_until_new_line_or_EOF.
 ** - it checks if there is a '\n' in the line_buffer,
 ** extracts the second part after the '\n' and stores it in next_line,
 ** and returns the first part.
 ** If the end of file is reached, it calls handle_error_or_EOF.
 **
 ** Return: The line_buffer or NULL.
 */
char	*get_next_line(int fd)
{
	static t_fd_node *fd_list_head = NULL;
	t_fd_node *fd_node;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	fd_node = get_or_add_node(fd, &fd_list_head);
	if (!fd_node)
		return (NULL);
	if (fd_node->next_line)
	{
		fd_node->line_buffer = fd_node->next_line;
		fd_node->next_line = NULL;
		if (ft_strchr(fd_node->line_buffer, '\n', &fd_node->char_read))
			return (ft_split(&fd_node->line_buffer, &fd_node->next_line, (fd_node->char_read + 1)));
	}
	while (1)
	{
		fd_node->line_buffer = read_until_new_line_or_EOF(fd_node, &fd_list_head);
		if (!fd_node->line_buffer)
			return (NULL);
		if (ft_strchr(fd_node->line_buffer, '\n', &fd_node->char_read))
			return (ft_split(&fd_node->line_buffer, &fd_node->next_line, fd_node->char_read + 1));
//		return (fd_node->line_buffer);
	}
}
