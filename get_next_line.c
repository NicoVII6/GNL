/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndecotti <ndecotti@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 12:49:10 by ndecotti          #+#    #+#             */
/*   Updated: 2023/01/17 14:00:03 by ndecotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*fill_line(int fd, char *left_c, char *buf);
static char	*set_line(char *line_buffer);
static char	*ft_strchr(char *s, int c);

/* checks if there is any problem with the file and
 * with the memory allocation
 * then it calls the function fill_line to read characters from
 * the file descriptor and stock it until the read pointer finds a \n or \0 
 * Once the variable is filled, we free the buffer to avoid any leaks 
 * and because we don't need it until the next call
 * then we fill the line with the function set_line and we return the line
 * and we store the remaining characters in left_c so next time GNL is called,
 * we have access to the characters that may have been read before */

char	*get_next_line(int fd)
{
	static char	*left_c;
	char		*line;
	char		*buf;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(left_c);
		free(buf);
		left_c = NULL;
		buf = NULL;
		return (NULL);
	}
	if (!buf)
		return (NULL);
	line = fill_line(fd, left_c, buf);
	free(buf);
	buf = NULL;
	if (!line)
		return (NULL);
	left_c = set_line(line);
	return (line);
}

/* this function fills the line buffer, it will read BUFFER_SIZE 
 * characters and iterate until it finds a \n or a \0 
 * each time through the loop, it will check if there is 
 * something in the left_c buffer.
 * if there is, it will add the new read characters to it; if not it will
 * duplicate the read buffer into left_c
 * if a \n is found, it will break out the loop and return the left_c buffer
 * after adding the read characters to it */

/* ssize_t is the same as size_t except it can received a negative number
 * since most of the functions might return a -1 if something fails, we use
 * ssize_t for the read_ptr */

static char	*fill_line(int fd, char *left_c, char *buf)
{
	ssize_t		read_ptr;
	char		*tmp;

	read_ptr = 1;
	while (read_ptr > 0)
	{
		read_ptr = (read(fd, buf, BUFFER_SIZE));
		if (read_ptr == -1)
		{
			free(left_c);
			return (NULL);
		}
		else if (read_ptr == 0)
			break ;
		buf[read_ptr] = 0;
		if (!left_c)
			left_c = ft_strdup("");
		tmp = left_c;
		left_c = ft_strjoin(tmp, buf);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (left_c);
}

/* function which takes the filled line and looks in it
 * until it find a \n or \0 character
 * this function sets to line_buffer a \0 to close it
 * and returns a substring named left_c which contains all 
 * the characters from the \n of the last line
 * read to the end of the buffer */

static char	*set_line(char *line_buffer)
{
	char	*left_c;
	ssize_t	i;

	i = 0;
	while (line_buffer[i] != '\n' && line_buffer[i] != '\0')
		i++;
	if (line_buffer[i] == 0 || line_buffer[1] == 0)
		return (NULL);
	left_c = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - i);
	if (*left_c == 0)
	{
		free(left_c);
		left_c = NULL;
	}
	line_buffer[i + 1] = 0;
	return (left_c);
}

/* cherche si \n se trouve dans le buffer qui vient d'etre lu */

static char	*ft_strchr(char *s, int c)
{
	int		i;
	char	character;

	character = (char) c;
	i = 0;
	while (s[i])
	{
		if (s[i] == character)
			return ((char *) &s[i]);
		i++;
	}
	return (NULL);
}
