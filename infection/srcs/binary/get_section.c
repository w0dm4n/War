/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_section.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

IMAGE_SECTION_HEADER	*get_section(unsigned char *buffer, char *name)
{
	IMAGE_NT_HEADERS *header = (IMAGE_NT_HEADERS*)&buffer[ ((IMAGE_DOS_HEADER*)buffer)->e_lfanew ];
	IMAGE_SECTION_HEADER *section = (IMAGE_SECTION_HEADER*)&buffer[((IMAGE_DOS_HEADER*)buffer)->e_lfanew + sizeof(IMAGE_NT_HEADERS)];
	for(int sectionIndex = 0; sectionIndex < header->FileHeader.NumberOfSections; sectionIndex++) {
		section = (IMAGE_SECTION_HEADER*)&buffer[ ((IMAGE_DOS_HEADER*)buffer)->e_lfanew + sizeof(IMAGE_NT_HEADERS) + (sectionIndex * sizeof(IMAGE_SECTION_HEADER))];
		if (strcmp(section->Name, name) == 0)
			return (section);
		printf("SECTION: %s\n", section->Name);
	}
	return NULL;
}
