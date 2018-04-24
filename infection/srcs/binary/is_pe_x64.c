/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_pe_x64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

/*
**	Check if the PE file is a 64bits executable
*/
bool	is_pe_x64(IMAGE_NT_HEADERS *pe)
{
	char	*value			= NULL;

	if (!(value = ft_strnew(MAGIC_LENGTH * 2)))
		return (false);
	if (pe != NULL && pe->OptionalHeader.Magic != 0)
		asprintf(&value, "%x", pe->OptionalHeader.Magic);
	if (strcmp(value, ARCHITECTURE_64) == 0)
	{
		free(value);
		return (true);
	}
	free(value);
	return (false);
}
