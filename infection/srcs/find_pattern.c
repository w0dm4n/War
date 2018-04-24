/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_pattern.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

static bool	is_pattern(char *file, int index, int len)
{
	char	path[] = { PATTERN };
	int		l = index + sizeof(path);
	int		i = 0;

	if (l > len) {
		return false;
	}
	while (i < sizeof(path))
	{
		if (file[index++] != path[i]) {
			return false;
		}
		i++;
	}
	return true;
}

int			find_pattern(char *file, int len)
{
	int		i = 0;
	char	path[] = { PATTERN };

	while (i < len - sizeof(path))
	{
		if (file[i] == path[0] && is_pattern(file, i, len)) {
			return i + sizeof(path);
		}
		i++;
	}
	return -1;
}
