/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_get_contents.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 13:27:27 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/08 13:35:06 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

static char		*read_contents(int fd, int size)
{
	int		ret;
	char	*buffer;

	buffer = ft_strnew(size);
	ret = read(fd, buffer, size);
	if (ret == -1)
	{
		free(buffer);
		return (ft_strnew(0));
	}
	return (buffer);
}

char			*file_get_contents(const char *filename)
{
	int			fd;
	struct stat	st;
	char		*result;

	if (stat(filename, &st) == -1)
		return (ft_strnew(0));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		return (ft_strnew(0));
	}
	result = read_contents(fd, st.st_size);
	close(fd);
	return (result);
}

static size_t	read_contents_size(char **content, int fd, int size)
{
	int		ret;

	*content = ft_strnew(size);
	ret = read(fd, *content, size);
	if (ret == -1)
	{
		free(*content);
		*content = ft_strnew(0);
		return (0);
	}
	return (ret);
}

size_t			file_get_contents_size(char **content, const char *filename)
{
	int			fd;
	struct stat	st;
	size_t		result;

	if (stat(filename, &st) == -1)
	{
		*content = ft_strnew(0);
		return (0);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		*content = ft_strnew(0);
		return (0);
	}
	result = read_contents_size(content, fd, st.st_size);
	close(fd);
	return (result);
}

size_t			file_get_contents_len_size(char **content, const char *filename, size_t size)
{
	int			fd;
	struct stat	st;
	size_t		result;

	if (stat(filename, &st) == -1)
	{
		*content = ft_strnew(0);
		return (0);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		*content = ft_strnew(0);
		return (0);
	}
	result = read_contents_size(content, fd, size);
	close(fd);
	return (result);
}
