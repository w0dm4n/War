/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_windows_binary_type.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

/*
** arg[0] = magic header of MAGIC_LENGTH
**
** Portable Executable return IS_PE_BINARY_TYPE
** MS-DOS Executable return IS_DOS_BINARY_TYPE
** ELSE IS_UNKNOW_BINARY_TYPE
*/
int 	get_windows_binary_type(char *header)
{
	char	*magic = NULL;
	char	*tmp = NULL;
	int		result = IS_UNKNOW_BINARY_TYPE;

	for (int i = 0; i < MAGIC_LENGTH; i++) {
		tmp = ft_strnew(10);
		asprintf(&tmp, "%x", header[i]);
		tmp[3] = '\0';
		magic = ft_dstrjoin(magic, tmp, 3);
	}
	magic = ft_strtoupper(magic);
	if (strcmp(magic, DOS_MAGIC) == 0) {
		result = IS_DOS_BINARY_TYPE;
	}
	if (strcmp(magic, PE_SIGNATURE) == 0) {
		result = IS_PE_BINARY_TYPE;
	}
	free(magic);
	return (result);
}
