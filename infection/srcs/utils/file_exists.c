/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_exists.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/11 20:25:06 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/11 20:25:08 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

BOOLEAN		file_exists(const char *filename)
{
	int			fd;
	struct stat	st;

	if (stat(filename, &st) == -1)
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		return (false);
	}
	close(fd);
	return (true);
}
