/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_archive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 19:24:26 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 21:22:09 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void			sort_binaries(t_ar_object *in, size_t *n_symbols)
{
	size_t			i;
	size_t			j;
	t_ar_object		swap;

	i = 0;
	while (i < *n_symbols)
	{
		j = i + 1;
		while (j < *n_symbols)
		{
			if (in[i].info.offset > in[j].info.offset)
			{
				swap = in[i];
				in[i] = in[j];
				in[j] = swap;
			}
			j++;
		}
		i++;
	}
}

void				list_archive(t_binary *data, char *path)
{
	size_t			i;
	t_ar_object		obj;
	char			*name;

	i = 0;
	sort_binaries(data->content.ar.contents, &data->content.ar.n_content);
	while (i < data->content.ar.n_content)
	{
		obj = data->content.ar.contents[i];
		name = obj.objname;
		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, path, ft_strlen(path));
		write(STDOUT_FILENO, "(", 1);
		write(STDOUT_FILENO, name, ft_strlen(name));
		write(STDOUT_FILENO, "):\n", 3);
		list_all_symbols(obj.bin, path);
		(void)name;
		(void)path;
		i++;
	}
}
