/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_windows_binary_file.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

/*
** arg[0] is absolute file_path of your file at check
**
** return true if file that PE or MS-DOS binary file
*/
bool	is_windows_binary_file(char *file_path) {
	char	*file = NULL;
	size_t	size = file_get_contents_len_size(&file, file_path, MAGIC_LENGTH);
	int		file_type;

	if (size < MAGIC_LENGTH) {
		free(file);
		return (IS_UNKNOW_BINARY_TYPE);
	}
	file_type = get_windows_binary_type(file);
	free(file);
	if (file_type != IS_UNKNOW_BINARY_TYPE)
		return (true);
	return (false);
}
