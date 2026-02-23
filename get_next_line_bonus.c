/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martafer <martafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:02:21 by martafer          #+#    #+#             */
/*   Updated: 2025/11/21 09:06:58 by martafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

// #include <fcntl.h>
// #include <stdio.h>

static char	*join_and_free(char *stash, char *temp, ssize_t bytes)
{
	char	*new_stash;

	if (bytes == -1)
	{
		free(temp);
		free(stash);
		return (NULL);
	}
	if (bytes == 0)
	{
		free(temp);
		return (stash);
	}
	if (ft_strlen(stash) == 0)
		new_stash = ft_strdup(temp);
	else
		new_stash = ft_strjoin(stash, temp);
	free(stash);
	return (new_stash);
}

static char	*read_to_stash(char *stash, int fd)
{
	char	*temp_stash;
	ssize_t	bytes;

	if (!stash)
		return (NULL);
	temp_stash = malloc(BUFFER_SIZE + 1);
	if (!temp_stash)
		return (NULL);
	bytes = 1;
	while (!ft_strchr(stash, '\n') && bytes > 0)
	{
		bytes = read(fd, temp_stash, BUFFER_SIZE);
		if (bytes <= 0)
			return (join_and_free(stash, temp_stash, bytes));
		temp_stash[bytes] = '\0';
		stash = join_and_free(stash, temp_stash, bytes);
		if (!stash)
		{
			free(temp_stash);
			return (NULL);
		}
	}
	free (temp_stash);
	return (stash);
}

static char	*extract_line(char *stash)
{
	char	*line;
	size_t	i;

	if (!stash || stash[0] == '\0')
		return (NULL);
	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*clean_stash(char *stash)
{
	size_t	i;
	size_t	j;
	char	*new_stash;

	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
		i++;
	if (stash[i] == '\0')
	{
		free (stash);
		return (NULL);
	}
	i++;
	new_stash = malloc(ft_strlen(stash) - i + 1);
	if (!new_stash)
		return (NULL);
	j = 0;
	while (stash[i] != '\0')
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 1023)
		return (NULL);
	if (!stash[fd])
		stash[fd] = ft_strdup("");
	stash[fd] = read_to_stash(stash[fd], fd);
	if (!stash[fd])
		return (NULL);
	line = extract_line(stash[fd]);
	stash[fd] = clean_stash(stash[fd]);
	return (line);
}

// int	main(void)
// {
// 	int	fd1 = open("tocho.txt", O_RDONLY);
// 	int	fd2 = open("test.txt", O_RDONLY);
// 	char	*line;
// 	int ok1 = 1;
// 	int ok2 = 1;
// 	while (ok1 || ok2)
// 	{
// 		line = get_next_line(fd1);
// 		printf("%s\n", line);
// 		if (line == NULL)
// 			ok1 = 0;
// 		free(line);line = get_next_line(fd2);
// 		printf("%s\n", line);
// 		if (line == NULL)
// 			ok2 = 0;
// 		free(line);
// 	}
// 	close(fd1);
// 	close(fd2);
// 	return (0);
// }
