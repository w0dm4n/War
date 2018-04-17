/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 19:50:57 by jguyet            #+#    #+#             */
/*   Updated: 2015/11/25 19:50:59 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"
#include <stdlib.h>

char	*ft_strnew(size_t size)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!(tmp = (char*)malloc(sizeof(char) * ((int)size + 1))))
		return (0);
	while (i < ((int)size + 1))
	{
		tmp[i] = '\0';
		i++;
	}
	return (tmp);
}
