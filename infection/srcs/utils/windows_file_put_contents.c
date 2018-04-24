/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   windows_file_put_contents.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 13:27:27 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/08 13:35:06 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

static bool	write_windows_contents(char *path, char *content, int len, bool create)
{
	HANDLE	file;
	DWORD	dwBytesToWrite = (DWORD)len;
	DWORD	dwBytesWritten;

	if ((file = CreateFile(path, FILE_GENERIC_WRITE, 0, NULL, create == true ? CREATE_ALWAYS : OPEN_EXISTING, // If you wish to create a file use CREATE_ALWAYS or CREATE_NEW instead of OPEN_EXISTING.
		 FILE_ATTRIBUTE_NORMAL, 0)) == NULL) {
		return (false);
	}
	WriteFile(file, content, dwBytesToWrite, &dwBytesWritten, NULL);
	if (dwBytesWritten <= 0) {
		return (false);
	}
	CloseHandle(file);
	return (true);
}

bool		windows_file_put_contents(char *path, char *content)
{
	return write_windows_contents(path, content, strlen(content), false);
}

bool		windows_file_put_contents_size(char *path, char *content, int size)
{
	return write_windows_contents(path, content, size, false);
}

bool		windows_file_create_contents_size(char *path, char *content, int size)
{
	return write_windows_contents(path, content, size, true);
}
