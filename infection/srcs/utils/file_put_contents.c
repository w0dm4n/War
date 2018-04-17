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
#include <stdio.h>
#include <string.h>

static BOOLEAN write_contents(int fd, const char *content, size_t size)
{
	if (write(fd, content, size) == -1)
		return (false);
	return (true);
}

BOOLEAN		file_put_contents(const char *filename, const char *content)
{
	return file_put_contents_size(filename, content, strlen(content));
}

BOOLEAN		file_put_contents_size(const char *filename, const char *content, size_t size)
{
	int			fd;
	BOOLEAN		result;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		return (false);
	result = write_contents(fd, content, size);
	close(fd);
	return (result);
}
