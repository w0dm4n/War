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
**	is PE Signature from PE Header in hex
*/
bool				is_pe_signature(IMAGE_NT_HEADERS *hdr)
{
	char	*value	= get_pe_signature(hdr);

	if (value == NULL)
		return (false);
	if (strcmp(value, PE_SIGNATURE) == 0)
	{
		free(value);
		return (true);
	}
	free(value);
	return (false);
}
