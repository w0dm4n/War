/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/19 05:00:34 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/19 05:00:36 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

#include <stdlib.h>

void	free_array(char **array)
{
	size_t	i;
	size_t	size;

	i = 0;
	if (array == NULL)
		return ;
	size = array_length(array);
	while (i < size)
		free(array[i++]);
	free(array);
}
