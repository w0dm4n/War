/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_absolute_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/11 20:24:56 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/11 20:24:58 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

char			*file_absolute_path(const char *file_path)
{
	char		**split;
	char		*absolute_path;
	size_t		i;
	size_t		split_size;

	split = ft_split_string(file_path, "\\");
	i = 0;
	split_size = array_length(split);
	absolute_path = ft_strnew(0);
	while (i < (split_size - 1))
	{
		absolute_path = \
			ft_dstrjoin(ft_dstrjoin(absolute_path, split[i], 1), "\\", 1);
		i++;
	}
	free_array(split);
	return (absolute_path);
}
