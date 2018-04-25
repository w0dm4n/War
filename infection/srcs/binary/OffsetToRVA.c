/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OffsetToRVA.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

int		OffsetToRVA(unsigned char *buffer, DWORD offset)
{
	IMAGE_NT_HEADERS		*header = (IMAGE_NT_HEADERS*)&buffer[ ((IMAGE_DOS_HEADER*)buffer)->e_lfanew ];
	IMAGE_SECTION_HEADER	*section = (IMAGE_SECTION_HEADER*)&buffer[((IMAGE_DOS_HEADER*)buffer)->e_lfanew + sizeof(IMAGE_NT_HEADERS)];

	for (int sectionIndex = 0; sectionIndex < header->FileHeader.NumberOfSections; sectionIndex++) {
		section = (IMAGE_SECTION_HEADER*)&buffer[ ((IMAGE_DOS_HEADER*)buffer)->e_lfanew + sizeof(IMAGE_NT_HEADERS) + (sectionIndex * sizeof(IMAGE_SECTION_HEADER))];
		if ((offset > section->PointerToRawData) && (offset <= section->PointerToRawData + section->SizeOfRawData)) {
			return (offset + section->VirtualAddress) - section->PointerToRawData;
		}
	}
	return 0;
}
