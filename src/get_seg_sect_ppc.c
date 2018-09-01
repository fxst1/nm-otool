/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_seg_sect_ppc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 13:58:22 by fjacquem          #+#    #+#             */
/*   Updated: 2018/09/01 13:58:32 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int		get_names_ppc(t_symbol *s, uint8_t *buf, size_t *sect_index)
{
	uint32_t	n_section;
	uint8_t		*tmp;

	n_section = ft_swap_bytes(*(uint32_t*)(buf + 0x30));
	tmp = buf + 0x38;
	while (n_section--)
	{
		(*sect_index)--;
		if (*sect_index == 0)
		{
			s->segname = (char*)tmp;
			s->sectname = (char*)tmp + 0x10;
			return (0);
		}
		tmp += 0x44;
	}
	return (1);
}

int				get_seg_sect_name_ppc(uint8_t *buf, size_t sect_index,
					t_symbol *s, size_t n_load_commands)
{
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;

	i = 0;
	buf += 0x1C;
	while (i < n_load_commands)
	{
		type = ft_swap_bytes(*((uint32_t*)buf));
		size = ft_swap_bytes(*((uint32_t*)buf + 1));
		if (type == LC_SEGMENT)
		{
			if (get_names_ppc(s, buf, &sect_index) == 0)
				return (0);
		}
		buf += size;
		i++;
	}
	return (1);
}
