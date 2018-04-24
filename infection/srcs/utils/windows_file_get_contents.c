/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   windows_file_get_contents.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 13:27:27 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/08 13:35:06 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

static char	*get_windows_file_content(char *path, int len)
{
	char		*content	= ft_strnew(len);
	HANDLE		file		= NULL;
	HANDLE		mapping		= NULL;
	LPCTSTR		pBuf		= NULL;

	if ((file = CreateFile(path, GENERIC_READ,
	FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)) == NULL) {
		return ft_strnew(0);
	}
	if ((mapping = CreateFileMapping(file, 0, PAGE_READONLY, 0, len, 0)) == NULL) {
		return ft_strnew(0);
	}
	if ((pBuf = (LPCTSTR)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, len)) == NULL) {
		return ft_strnew(0);
	}
	if (pBuf == NULL) {
		CloseHandle(file);
		return ft_strnew(0);
	}
	memcpy(content, pBuf, len);
	UnmapViewOfFile(pBuf);
	CloseHandle(file);
	return (content);
}

char	*windows_file_get_contents(char *path)
{
	int			len			= 0;
	char		*content	= NULL;
	int			fd			= 0;

	if ((fd = open(path, O_RDWR)) == -1) {
		return ft_strnew(0);
	}
	if ((len = lseek(fd, 0, SEEK_END)) <= 0) {
		return ft_strnew(0);
	}
	close (fd);
	if ((content = get_windows_file_content(path, len)) == NULL)
		return ft_strnew(0);
	return (content);
}

size_t 	windows_file_get_contents_size(char **content, char *path)
{
	int			len			= 0;
	int			fd			= 0;

	if ((fd = open(path, O_RDWR)) == -1) {
		*content = ft_strnew(0);
		return 0;
	}
	if ((len = lseek(fd, 0, SEEK_END)) <= 0) {
		*content = ft_strnew(0);
		return 0;
	}
	close (fd);
	if ((*content = get_windows_file_content(path, len)) == NULL) {
		*content = ft_strnew(0);
		return 0;
	}
	return (len);
}
