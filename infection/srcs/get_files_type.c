/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

static char		**add_element(char **tab, char *element)
{
	char	**ntab;
	int		i;

	if (!(ntab = (char**)malloc(sizeof(char*)*(array_length(tab) + 2))))
		return (NULL);
	i = 0;
	while (tab != NULL && tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	ntab[i] = element;
	ntab[i + 1] = NULL;
	if (tab != NULL)
		free(tab);
	return (ntab);
}

char			**get_files_types(char **files, char *start_path, bool f(char *file_path))
{
	DIR				*directory = opendir(start_path);
	struct dirent	*d;
	char			*absolute_path;
	char			*file_path;
	char			**dirs;

	if (!directory)
		return (NULL);
	dirs = NULL;
	absolute_path = strdup(start_path);
	while ((d = readdir(directory)) != NULL)
	{
		if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
			continue ;
		file_path = ft_dstrjoin(ft_dstrjoin(absolute_path, "\\", 0), d->d_name, 1);
		if (file_exists(file_path) == false && is_directory(file_path) == false)
		{
			free(file_path);
			continue ;
		}
		if (is_directory(file_path) == true)
		{
			dirs = add_element(dirs, strdup(file_path));
			free(file_path);
			continue ;
		}
		if (f(file_path) == true)
		{
			files = add_element(files, strdup(file_path));
		}
		free(file_path);
	}

	int i = 0;
	while (i < (int)array_length(dirs))
	{
		files = get_files_types(files, dirs[i], f);
		free(dirs[i]);
		i++;
	}
	if (dirs != NULL)
		free(dirs);
	closedir(directory);
	return (files);
}
