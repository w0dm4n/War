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
**	Get PE Signature from PE Header in hex
*/
char				*get_pe_signature(IMAGE_NT_HEADERS *hdr)
{
	char	*signature = NULL;

	if (!(signature = ft_strnew(MAGIC_LENGTH * 2)))
		return (NULL);
	if (hdr != NULL && hdr->Signature != 0)
		asprintf(&signature, "%x", hdr->Signature);
	return (signature);
}
