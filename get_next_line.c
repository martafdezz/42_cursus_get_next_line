/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martafer <martafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:02:21 by martafer          #+#    #+#             */
/*   Updated: 2026/02/18 20:36:45 by martafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <fcntl.h>
#include <stdio.h>

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
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
		stash = ft_strdup("");
	stash = read_to_stash(stash, fd);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = clean_stash(stash);
	return (line);
}

int	main(void)
{
	int	fd = open("1char.txt", O_RDONLY);
	char	*line;
	while (1)
	{
		line = get_next_line(fd);
		printf("%s\n", line);
		if (line == NULL)
			break;
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}
