/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_get_n_section_name.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:53:31 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 17:55:14 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static char		**set_names(char **names, char *sectname, char *segname)
{
	names[1] = sectname;
	names[0] = segname;
	return (names);
}

static char		**mach64(t_mach64 *macho, size_t section_index, char **names)
{
	size_t		i;
	size_t		j;
	size_t		index;

	i = 0;
	index = 0;
	while (i < macho->header.ncmds)
	{
		if (macho->cmds[i].cmd == LC_SEGMENT_64)
		{
			j = 0;
			while (j < macho->cmds[i].nsects)
			{
				if (section_index == index)
				{
					return (set_names(names,
						macho->cmds[i].sections[j].sectname,
						macho->cmds[i].sections[j].segname));
				}
				index++;
				j++;
			}
			i++;
		}
	}
	return (names);
}

static char		**mach32(t_mach32 *macho, size_t section_index, char **names)
{
	size_t		i;
	size_t		j;
	size_t		index;

	i = 0;
	index = 0;
	while (i < macho->header.ncmds)
	{
		if (macho->cmds[i].cmd == LC_SEGMENT)
		{
			j = 0;
			while (j < macho->cmds[i].nsects)
			{
				if (section_index == index)
				{
					return (set_names(names,
						macho->cmds[i].sections[j].sectname,
						macho->cmds[i].sections[j].segname));
				}
				index++;
				j++;
			}
			i++;
		}
	}
	return (names);
}

char			**mach_get_n_section_name(t_binary *bin, size_t section_index)
{
	char		**names;

	names = malloc(sizeof(char*) * 2);
	names[0] = "";
	names[1] = "";
	if (section_index == 0)
		return (names);
	if (bin->type_id == TYPE_ID_MACH64)
		return (mach64(&bin->content.mach64, section_index - 1, names));
	else if (bin->type_id == TYPE_ID_MACH32)
		return (mach32(&bin->content.mach32, section_index - 1, names));
	return (names);
}
