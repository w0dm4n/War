/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_base_name.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/11 20:25:06 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/11 20:25:08 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

#include <string.h>

char			*file_base_name(const char *file_path)
{
	char		**split;
	char		*base_name;
	size_t		split_size;

	split = ft_split_string(file_path, "/");
	split_size = array_length(split);
	if (split_size <= 0)
	{
		free_array(split);
		return (ft_strnew(0));
	}
	base_name = strdup(split[split_size - 1]);
	free_array(split);
	return (base_name);
}
