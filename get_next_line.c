/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndecotti <ndecotti@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:10:17 by ndecotti          #+#    #+#             */
/*   Updated: 2023/01/13 11:10:17 by ndecotti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>

static char *fill_line(int fd, char *left_c, char *buf);
static char	*set_line(char *line_buffer);
static char	*ft_strchr(char *s, int c);

char	*get_next_line(int fd)
{
	/* checks if there is any problem with the file descriptor and with the memory allocation 
	 * Once checks are done, it calls the function fill_line to read characters from the file
	 * descriptor until the read pointer finds a \n or \0 
	 * Once the variable is filled, we free the buffer to avoid any leaks and because we don't need it
	 * until the next call 
	 * Once the buffer is freed, we fill the line with the function make_line and we return the line
	 * and we store the line in a static variable so next time get_next_line is called, we have access
	 * to the characters that may have been read before */

	static char	*left_c;
	char		*line;
	char		*buf;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	/* read(fd, 0, 0) < 0 will check if the file descriptor exist and it has some contents to read
	 */
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(buf);
		free(left_c);
		buf = NULL;
		left_c = NULL;
		return (NULL);
	}
	if (!buf)
		return (NULL);
	line = fill_line(fd, left_c, buf);
	free(buf);
	buf = NULL;
	if (!line)
		return (NULL);
	/* on envoie la ligne built with fill_line to set_line to check the \n of it
	 * and divide it accordingly */
	left_c = set_line(line);
	return (line);
	/* on retourne la ligne \n terminating et on garde le reste (s'il y a) dans left_c 
	 * ready to be filled for the next call of GNL */
}

/* this function fills the line buffer, it will read BUFFER_SIZE characters and iterate
 * until it finds a \n or a \0 
 * each time through the loop, it will check if there is something in the left_c buffer.
 * if there is, it will add the new read characters to it; if not it will duplicate the read buffer
 * into the left_c buffer 
 * if a \n is found, it will break out the loop and return the left_c buffer after adding the read
 * characters to it */

static char *fill_line(int fd, char *left_c, char *buf)
{
	/* ssize_t is the same as size_t except it can received a negative number
	 * since most of the functions might return a -1 if something fails, we use
	 * ssize_t for the read_ptr */

	ssize_t		*read_ptr;
	char		*tmp;

	read_ptr = 1;
	while (read_ptr > 0)
	{
		read_ptr = read(fd, buf, BUFFER_SIZE);
		if (read_ptr == -1)
		{
			free(left_c);
			return (NULL);
		}
		/* it means that all the characters have been read so we are at the end of the file
		 * we can get out of the loop */
		else if (read_ptr == 0)
			break ;
		// we set the last character 0 to the buffer to NULL terminate the string
		buf[read_ptr] = 0;
		/* now we check if the left_c static char pointer is empty, because if so, we cannot use the 
		 * strjoin function */
		if (left_c == 0)
			left_c == ft_strdup("");
		tmp = left_c; // a chaque nouveau loop, tmp prend la valeur de tout ce qui a été copié précedemment sur left_c
		/* once we set left_c to be a null character string or it is fill with some characters
		 * from the last call of get_next_line assign tmp variable the left_c value
		 * we can joined we just read to left_c */
		left_c = strjoin(tmp, buf);
		/* a chaque loop, on ajoute le contenu du buffer pour sur la ligne en cours */
		free(tmp);
		tmp = NULL;
		/* we search if there is any new line character into the buffer we just read
		 * and if so, we just go out of the loop */
		if (ft_strchr(buf, '\n');
			break ; 
	}
	return (left_c); // que deviens buf une fois joint ac tmp ? buffer free ds main function !
}

/* function which takes the line buffer as parameter and reads it until it find
 * a \n or \0 character 
 * this function sets to line_buffer a \0 to close it and returns a substring named
 * left_c which contains all the characters from the \n of the last line read to the
 * end of the buffer */

static char	*set_line(char *line_buffer)
{
	char	*left_c;
	ssize_t	i;

	i = 0;
	while (line_buffer[i] != '\n' && line_buffer[i] != '\0')
		i++;
	if (line_buffer[i] == 0 || line_buffer[1] == 0)
		return (NULL);
	/* if we reach the end of the file or if the line is empty 
	 * we return NULL because there is no line to set */
	left_c = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - i);
	/* we copie the substring of the part from the \n of line_buffer to the end
	 * of the whole line_buffer */
	if (*left_c == 0)
	{
		free(left_c);
		left_c = NULL;
	}
	line_buffer[i + 1] = 0
	/* close the line with the null character but why ?? */
	return (left_c);
}

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
	}
	return (NULL);
}