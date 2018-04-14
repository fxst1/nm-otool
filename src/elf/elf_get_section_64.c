/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_get_section_64.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 17:03:48 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 20:32:10 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void				elf_get_section_64(t_binary *bin, char *sectname,
										t_section_info *sect)
{
	size_t			i;
	size_t			n;
	char			*name;
	t_elf64_section	*sects;

	sect->size = 0;
	sect->offset = 0;
	i = 0;
	n = bin->content.elf64.header.shnum;
	sects = bin->content.elf64.sections;
	while (i < n)
	{
		name = bin->content.elf64.shstrtab + sects[i].name;
		if (ft_strcmp(name, sectname) == 0)
		{
			sect->size = sects[i].size;
			sect->offset = sects[i].offset;
		}
		i++;
	}
}
